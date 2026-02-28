#include "sessionmanager.h"
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include "self.h"

SessionManager::SessionManager(Self *user, QObject *parent)
  : QObject{parent}
  , m_user(user)
  , m_peerModel(new PeerListModel(this))
{
  loadSavedPeers();
}

bool SessionManager::loadSavedPeers()
{
  QFile file(Self::savedPeersFilePath);

  if (!file.exists()) {
    m_user->createFiles();
  }

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Something went wrong while reading saved files:" << file.errorString();
    return false;
  }

  QByteArray data = file.readAll();
  QJsonParseError error;
  QJsonDocument doc(QJsonDocument::fromJson(data, &error));

  if (doc.isNull()) {
    qWarning() << "JSON document is empty:" << error.errorString();
    return false;
  }

  QJsonArray json = doc.array();

  for (const QJsonValueRef &entry : json) {
    QJsonObject peerJson = entry.toObject();
    Peer *peer = new Peer(m_user, this);
    peer->setName(peerJson["name"].toString());
    peer->setAddressAndPort(peerJson["addr"].toString(), peerJson["port"].toInt());
    m_peerMap.insert(peer->id(), peer);
    m_peerModel->addPeer(peer);
  }

  return true;
}

void SessionManager::addPeer(Peer *peer)
{
  m_peerMap.insert(peer->id(), peer);
  m_peerModel->addPeer(peer);
}

void SessionManager::deletePeer(Peer *peer)
{
  m_peerMap.remove(peer->id());
}

void SessionManager::activatePeer(PeerId id, QTcpSocket *conn)
{
  Peer *peer = m_peerMap.value(id);
  peer->setConn(conn);
  connect(peer->conn(), &QTcpSocket::readyRead, peer, &Peer::handleMessage);
}

PeerListModel *SessionManager::peers() const
{
  return m_peerModel;
}

void SessionManager::saveChat(Peer *peer, QAbstractListModel *msgModel)
{
  QHash<int, QByteArray> roles = msgModel->roleNames();
  int sentAttr = roles.key("sent");
  int textAttr = roles.key("text");

  QJsonArray chat{};
  int chatSize = msgModel->rowCount();

  for (int i = 0; i < chatSize; ++i) {
    QModelIndex idx = msgModel->index(i, 0);
    bool sent = msgModel->data(idx, sentAttr).toBool();
    QString text = msgModel->data(idx, textAttr).toString();
    Message msg(peer->id(), sent, text);
    chat.append(msg.toJson());
  }

  // save chat to file
  QDir dir(Self::userConfigDir);

  if (!dir.exists())
    dir.mkpath(".");

  QFile file(Self::userConfigDir + "/" + peer->name());

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qErrnoWarning("Can't open file");
    return;
  }

  QJsonDocument doc(chat);
  file.write(doc.toJson());
  file.close();
}

QList<Message> SessionManager::loadChat(Peer *peer)
{
  QFile file(Self::userConfigDir + "/" + peer->name());
  QList<Message> messages;

  if (!file.open(QIODevice::ReadOnly)) {
    qErrnoWarning("Can't open file");
    return messages;
  }

  QByteArray fileData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(fileData);
  QJsonArray chat = doc.array();

  for (const QJsonValue &json : chat) {
    Message msg(peer->id(), json["sent"].toBool(), json["data"].toString());
    messages.append(msg);
  }

  return messages;
}

void SessionManager::addNewMsgTo(Peer *peer, bool sent, QString msg)
{
  // todo
}
