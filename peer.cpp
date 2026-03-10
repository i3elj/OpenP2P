#include "peer.h"
#include <QJsonObject>
#include "message.h"

Peer::Peer(QTcpSocket *conn, QObject *parent)
  : QObject{parent}
  , m_id()
  , m_name("")
  , m_addr()
  , m_port(0)
  , m_conn(conn)
  , m_active(false)
{
  m_conn->setParent(this);
  m_addr = m_conn->peerAddress();
  connect(m_conn, &QTcpSocket::connected, this, &Peer::activate);
  connect(m_conn, &QTcpSocket::disconnected, this, &Peer::deactivate);
}

Peer::Peer(QObject *parent)
  : QObject{parent}
  , m_id()
  , m_name("")
  , m_addr()
  , m_port(0)
  , m_conn(new QTcpSocket(this))
  , m_active(false)
{
  connect(m_conn, &QTcpSocket::connected, this, &Peer::activate);
  connect(m_conn, &QTcpSocket::disconnected, this, &Peer::deactivate);
}

PeerId Peer::id() const
{
  return m_id;
}

QString Peer::name() const
{
  return m_name;
}

QString Peer::addr() const
{
  return m_addr.toString();
}

int Peer::port() const
{
  return m_port;
}

bool Peer::isActive() const {
  return m_active;
}

QTcpSocket *Peer::conn() const
{
  return m_conn;
}

void Peer::setName(QString n)
{
  if (m_name == n)
    return;
  m_name = n;
  emit nameChanged();
}

bool Peer::setAddr(QString address)
{
  QHostAddress addr;

  if (!addr.setAddress(address) || addr.protocol() != QAbstractSocket::IPv6Protocol) {
    return false;
  }

  m_addr = addr;
  return true;
}

void Peer::setPort(int port)
{
  m_port = port;
  m_id = PeerId(m_addr, m_port);
}

void Peer::setConn(QTcpSocket *conn)
{
  m_conn = conn;
  m_conn->setParent(this);
  connect(m_conn, &QTcpSocket::connected, this, &Peer::activate);
  connect(m_conn, &QTcpSocket::disconnected, this, &Peer::deactivate);

  if (m_conn->state() == QTcpSocket::ConnectedState) {
    activate();
  }
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

bool Peer::setAddressAndPort(QString address, int port)
{
  QHostAddress addr;

  if (!addr.setAddress(address) || addr.protocol() != QAbstractSocket::IPv6Protocol) {
    return false;
  }

  m_addr = addr;
  m_port = port;
  m_id = PeerId(m_addr, m_port);
  return true;
}

void Peer::connectToHost()
{
  m_conn->connectToHost(m_addr, m_port);
}

void Peer::sendMsg(QString txt) // reimplement
{
  Message msg(true, txt);
  sendMsg(msg);
}

void Peer::sendMsg(Message msg) // reimplement
{
  qint64 bytes = m_conn->write(msg.toBytes());
  qWarning() << "Bytes written:" << bytes << ". Success? " << (bytes != -1);
  emit msgSent(msg.text(), bytes != -1);
}

/**
 * Slot functions.
 */

void Peer::handle()
{
  Message req(m_conn->readAll());

  if (req.type() == Message::Type::DataExchange) {
    setName(req.hostName());
    return;
  }

  qWarning() << "Message received from" << m_name << ", said:" << req.text();
  emit newMsg(req.text());
}

void Peer::activate()
{
  m_active = true;
  emit activeChanged();
}

void Peer::deactivate()
{
  m_active = false;
  m_conn->deleteLater();
  emit activeChanged();
}
