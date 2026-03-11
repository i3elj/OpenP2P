#include "server.h"

/**
 * Private Functions
 */

void Server::onRemoteRejected(Peer *peer)
{
  disconnect(peer->conn(), &QTcpSocket::connected, nullptr, nullptr);
  disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);
  m_session->deletePeer(peer);
  peer->deleteLater();
}

void Server::onRemoteAccepted(Peer *peer, Message acceptMsg)
{
  peer->setName(acceptMsg.hostName());
  peer->setPort(acceptMsg.Port());
  finalizePeerConnection(peer);
}

void Server::finalizePeerConnection(Peer *peer)
{
  disconnect(peer->conn(), &QTcpSocket::connected, nullptr, nullptr);
  disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);

  if (!m_session->contains(peer->id())) {
    m_session->addPeer(peer);
  }

  peer->setupConnection();
  peer->setup();
  emit peerConnectionFinalized(peer);
}

void Server::exchangeData(Peer *peer)
{
  connect(peer->conn(), &QTcpSocket::connected, this, [this, peer]() {
    Message req(Message::Type::DataExchange);
    req.setMetaData(m_user->name(), m_user->port());
    peer->sendMsg(req);
  });

  connect(peer->conn(), &QTcpSocket::readyRead, this, [this, peer]() {
    Message req(peer->conn()->readAll());

    if (req.type() == Message::Type::Reject)
      onRemoteRejected(peer);
    if (req.type() == Message::Type::Accept)
      onRemoteAccepted(peer, req);
  });
}

void Server::tryReconnecting(Peer *peer)
{
  if (peer->isActive()) {
    emit peerAlreadyConnected(peer->id());
  } else {
    peer->setConn();
    peer->connectToHost();
    exchangeData(peer);
  }
}

/**
 * Public Functions
 */

Server::Server(Self *user, SessionManager *sm, QObject *parent)
  : QTcpServer{parent}
  , m_ipr(new IPv6AddrResolver(this))
  , m_session(sm)
  , m_user(user)
{
  AddressList addresses = m_ipr->resolve();
  listen(addresses.first(), m_user->port());
}

void Server::initTcpSocket()
{
  connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

/**
 * Q_INVOKABLE Functions
 */

void Server::tryReconnectAll()
{
  for (const auto& peer : m_session->getAllPeers()) {
    tryReconnecting(peer);
  }
}

void Server::acceptIncomingPeer(Peer *peer)
{
  finalizePeerConnection(peer);
  Message res(Message::Type::Accept);
  res.setMetaData(m_user->name(), m_user->port());
  peer->sendMsg(res);
  peer->activate();
}

void Server::rejectIncomingPeer(Peer *peer)
{
  disconnect(peer->conn(), &QTcpSocket::connected, nullptr, nullptr);
  disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);

  peer->close();
  peer->deleteLater();
}

void Server::startNewConn(QString address, int port)
{
  PeerId id(QHostAddress(address), port);

  if (m_session->contains(id)) {
    tryReconnecting(m_session->getPeer(id));
    return;
  }

  Peer *peer = new Peer(this);

  if (!peer->setAddressAndPort(address, port)) {
    emit wrongAddress(address);
    peer->deleteLater();
    return;
  }

  exchangeData(peer);
  peer->connectToHost();
}

/**
 * Slots Functions
 */

void Server::handleNewConnection()
{
  QTcpSocket *conn = nextPendingConnection();
  Peer *peer = new Peer(conn, this);

  connect(peer->conn(), &QTcpSocket::readyRead, this, [this, peer]() {
    Message req(peer->conn()->readAll());

    if (req.type() == Message::Type::DataExchange) {
      auto json = req.toJson();
      peer->setName(json[Self::SettingsKeys::Name].toString());
      peer->setPort(json[Self::SettingsKeys::Port].toInt());

      Peer *sessPeer = m_session->getPeer(peer->id());

      if (sessPeer != nullptr) {
        if (sessPeer->isActive()) {
          peer->deleteLater();
          qWarning() << "Peer is already connected and online";
          return;
        }

        disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);
        sessPeer->setConn(peer->conn());
        Message res(Message::Type::Accept);
        res.setMetaData(m_user->name(), m_user->port());
        sessPeer->sendMsg(res);
        sessPeer->setup();
        peer->deleteLater();
        return;
      }

      emit newConnection(peer);
    }
  });
}
