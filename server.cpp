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

void Server::startNewConn(QString address, int port)
{
  Peer peer(m_user, this);

  if (!peer.setAddressAndPort(address, port)) {
    emit wrongAddress(address);
  }

  connect(&peer, &Peer::accepted, this, &Server::acceptPeer);
  connect(&peer, &Peer::rejected, this, &Server::destroyPeer);

  peer.connectToHost();
  peer.sendMsg(Message());
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

void Server::destroyPeer(Peer *peer) {
  disconnect(peer);
  m_session->deletePeer(peer);
  peer->deleteLater();
}

void Server::setupPeer(Peer *peer)
{
  m_session->activatePeer(peer->id(), peer->conn());
  peer->setup();
}

void Server::acceptPeer(Peer *peer)
{
  disconnect(peer);
  m_session->addPeer(peer);
  peer->setup();
}

void Server::rejectPeer(Peer *peer)
{
  peer->close();
  peer->deleteLater();
}
