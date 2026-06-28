#include "peer.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include "self.h"

Peer::Peer(Self* user, QTcpSocket *conn, QObject *parent)
  : QObject{parent}
  , m_id()
  , m_name("")
  , m_addr()
  , m_port(0)
  , m_conn(conn)
  , m_active(false)
  , m_chatModel(new ChatListModel(this))
  , m_user(user)
  , m_pubKey(nullptr)
  , m_crypto()
{
  m_conn->setParent(this);
  m_addr = m_conn->peerAddress();
  connect(this, &Peer::connectionChanged, this, &Peer::setupConnection);
  connect(this, &Peer::msgSent, m_chatModel, &ChatListModel::add);
  connect(this, &Peer::newMsg, m_chatModel, &ChatListModel::add);
}

Peer::Peer(Self *user, QObject *parent)
  : QObject{parent}
  , m_id()
  , m_name("")
  , m_addr()
  , m_port(0)
  , m_conn(new QTcpSocket(this))
  , m_active(false)
  , m_chatModel(new ChatListModel(this))
  , m_user(user)
  , m_pubKey(nullptr)
  , m_crypto()
{
  connect(this, &Peer::connectionChanged, this, &Peer::setupConnection);
  connect(this, &Peer::msgSent, m_chatModel, &ChatListModel::add);
  connect(this, &Peer::newMsg, m_chatModel, &ChatListModel::add);
}

Peer::~Peer() {
  EVP_PKEY_free(m_pubKey);
  saveChat();
}

PeerId Peer::id() const { return m_id; }

QString Peer::name() const { return m_name; }

QString Peer::addr() const { return m_addr.toString(); }

int Peer::port() const { return m_port; }

QTcpSocket *Peer::conn() const { return m_conn; }

bool Peer::isActive() const { return m_active; }

ChatListModel *Peer::chatModel() const { return m_chatModel; }

QString Peer::publicKey() const { return m_crypto.keyToPEM(m_pubKey); }

void Peer::setName(QString n) {
  if (m_name == n)
    return;
  m_name = n;
  emit nameChanged();
}

bool Peer::setAddr(QString address) {
  QHostAddress addr;

  if (!addr.setAddress(address) || addr.protocol() != QAbstractSocket::IPv6Protocol) {
    return false;
  }

  m_addr = addr;
  return true;
}

void Peer::setPort(int port) {
  m_port = port;
  m_id = PeerId(m_addr, m_port);
}

void Peer::setConn(QTcpSocket *conn) {
  m_conn = conn;
  m_conn->setParent(this);
  emit connectionChanged();

  if (m_conn->state() == QTcpSocket::ConnectedState) {
    activate();
  }
}

void Peer::setPublicKey(QString keyStr) {
  EVP_PKEY *key = m_crypto.PEMtoKey(keyStr);

  if (!key) {
    qWarning() << "Provided key is improper or incorrect";
    return;
  }

  EVP_PKEY_free(m_pubKey);
  m_pubKey = key;
}

void Peer::setupHandler() {
  connect(m_conn, &QTcpSocket::readyRead, this, &Peer::handle);
  activate();
}

void Peer::close() {
  m_conn->write("Rejected");
  m_conn->abort();
}

bool Peer::setAddressAndPort(QString address, int port) {
  QHostAddress addr;

  if (!addr.setAddress(address) || addr.protocol() != QAbstractSocket::IPv6Protocol) {
    return false;
  }

  m_addr = addr;
  m_port = port;
  m_id = PeerId(m_addr, m_port);
  return true;
}

void Peer::connectToHost() { m_conn->connectToHost(m_addr, m_port); }

void Peer::sendMsg(Message msg, bool encrypt) {
  msg.setSent(true);
  QByteArray byteMsg = msg.toBytes();
  QByteArray data = encrypt ? m_user->encrypt(m_pubKey, byteMsg) : byteMsg;
  qint64 bytes = m_conn->write(data);

  if (bytes != -1 && msg.type() == MessageType::Common)
    emit msgSent(msg);
}

void Peer::loadMessages(QList<Message> messages) {
  m_chatModel->reset(messages);
}

/**
 * Q_INVOKABLE FUNCTIONS
 */

void Peer::sendMsg(QString text) { sendMsg(Message(true, text), true); }

void Peer::saveChat() {
  QHash<int, QByteArray> roles = m_chatModel->roleNames();
  int sentAttr = roles.key(Self::SettingsKeys::Sent);
  int textAttr = roles.key(Self::SettingsKeys::Text);

  QJsonArray chat{};
  int chatSize = m_chatModel->rowCount();

  for (int i = 0; i < chatSize; ++i) {
    QModelIndex idx = m_chatModel->index(i, 0);
    bool sent = m_chatModel->data(idx, sentAttr).toBool();
    QString text = m_chatModel->data(idx, textAttr).toString();
    Message msg(sent, text);
    chat.append(msg.toJson());
  }

  // save chat to file
  QDir dir(Self::userConfigDir);

  if (!dir.exists())
    dir.mkpath(".");

  QFile file(Self::userConfigDir + m_name);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qErrnoWarning("Can't open file");
    return;
  }

  QJsonDocument doc(chat);
  file.write(doc.toJson());
  file.close();
}

/**
 * Slot functions.
 */

void Peer::handle() {
  QByteArray data(m_conn->readAll());
  Message req(m_user->decrypt(data));
  // Message req(m_conn->readAll());
  req.setSent(false);

  if (req.type() == MessageType::DataExchange) {
    setName(req.hostName());
    return;
  }

  if (req.type() == MessageType::Common) {
    qWarning() << "Message received from" << m_name << ", said:" << req.text();
    emit newMsg(req);
  }
}

void Peer::activate() {
  m_active = true;
  emit activeChanged();
}

void Peer::deactivate() {
  m_active = false;
  m_conn->deleteLater();
  emit activeChanged();
}

void Peer::setupConnection() {
  connect(m_conn, &QTcpSocket::connected, this, &Peer::activate);
  connect(m_conn, &QTcpSocket::disconnected, this, &Peer::deactivate);
  connect(m_conn, &QTcpSocket::errorOccurred, this, &Peer::deactivate);
}
