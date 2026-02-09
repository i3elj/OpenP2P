#include "server.h"

Server::Server(SessionManager *sm, QObject *parent)
  : QTcpServer{parent}
  , m_session(sm)
  , m_ipr(new IPv6AddrResolver(this))
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
  Peer *peer = new Peer(conn, id, this);
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

Server::~Server() {}
