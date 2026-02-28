#include "peer.h"
#include <QJsonObject>
#include "message.h"
#include "self.h"

Peer::Peer(Self *user, QTcpSocket *conn, PeerId id, QObject *parent)
  : QObject{parent}
  , m_id(id)
  , m_conn(conn)
  , m_user(user)
{
  m_conn->setParent(this);
  m_addr = m_conn->peerAddress();
  m_port = m_conn->peerPort();
}

Peer::Peer(Self *user, QObject *parent)
  : QObject{parent}
  , m_conn(new QTcpSocket())
  , m_user(user)
{
  m_conn->setParent(this);
}

void Peer::setup()
{
  connect(m_conn, &QTcpSocket::readyRead, this, &Peer::handleMessage);
}

void Peer::close()
{
  m_conn->write("Rejected");
  m_conn->abort();
}

PeerId Peer::id() const
{
  return m_id;
}

QTcpSocket *Peer::conn() const { return m_conn; }

void Peer::setConn(QTcpSocket *conn) {
  m_conn = conn;
}

QString Peer::addr() const
{
  return m_addr.toString();
}

int Peer::port() const
{
  return m_port;
}

QString Peer::name() const
{
  return m_name;
}

void Peer::setName(QString n)
{
  if (m_name == n)
    return;
  m_name = n;
  emit nameChanged();
}

bool Peer::setAddressAndPort(QString address, int port)
{
  QHostAddress addr;

  if (!addr.setAddress(address) || addr.protocol() != QAbstractSocket::IPv6Protocol) {
    return false;
  }

  m_addr = addr;
  m_port = port;
  return true;
}

void Peer::connectToHost()
{
  connect(m_conn, &QTcpSocket::readyRead, m_conn, [this]() {
    Message msg(m_conn->readAll());

    if (msg.type() == Message::Type::Reject) {
      disconnect(m_conn);
      emit rejected(this);
      return;
    }

    emit accepted(this);
  });

  // connect(m_conn, &QTcpSocket::errorOccurred, this, [](){});
  m_conn->connectToHost(m_addr, m_port);
}

void Peer::sendMsg(QString txt) // reimplement
{
  Message payload(id(), true, txt);
  qint64 bytes = m_conn->write(payload.toBytes());
  emit msgSent(this, txt, bytes != -1);
}

void Peer::sendMsg(Message msg) // reimplement
{
  qint64 bytes = m_conn->write(msg.toBytes());
  emit msgSent(this, msg.toBytes(), bytes != -1);
}

void Peer::handleMessage()
{
  Message payload(m_conn->readAll());
  emit newMsg(this, payload.text());
}
