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

void Peer::setup()
{
  connect(m_conn, &QTcpSocket::readyRead, this, &Peer::handle);
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

void Peer::sendMsg(QString msg)
{
  Message payload(id(), true, msg);
  qint64 bytes = m_conn->write(payload.toBytes());
  emit msgSent(this, msg, bytes != -1);
}

void Peer::handle()
{
  Message payload(m_conn->readAll());
  emit newMsg(this, payload.message());
}
