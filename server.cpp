#include "server.h"

Server::Server(Self *user, SessionManager *sm, QObject *parent)
  : QTcpServer{parent}
  , m_ipr(new IPv6AddrResolver(this))
  , m_session(sm)
  , m_user(user)
{
  AddressList addresses = m_ipr->resolve();
  listen(addresses.first(), 7755);
}

void Server::initTcpSocket()
{
  connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

void Server::handleNewConnection()
{
  QTcpSocket *conn = nextPendingConnection();
  PeerId id(conn);
  Peer *peer = new Peer(m_user, conn, id, this);
  emit newConnection(peer);
}

void Server::setupPeer(Peer *peer)
{
  m_session->addPeer(peer->id(), peer);
  peer->setup();

  connect(peer, &Peer::newMsg, this, &Server::newMsg);
}

void Server::rejectPeer(Peer *peer)
{
  peer->close();
  peer->deleteLater();
}
