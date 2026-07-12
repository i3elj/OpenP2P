#include "src/server.hpp"

/**
 * Private Functions
 */

void Server::onRemoteRejected(Peer *peer)
{
  disconnect(peer->conn(), &QTcpSocket::connected, nullptr, nullptr);
  disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);
  m_session->deletePeer(peer);
  m_session->saveAllPeers();
  peer->deleteLater();
}

void Server::onRemoteAccepted(Peer *peer, Message acceptMsg)
{
  peer->setName(acceptMsg.hostName());
  peer->setPort(acceptMsg.port());
  peer->setPublicKey(acceptMsg.publicKey());
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
  peer->setupHandler();
  emit peerConnectionFinalized(peer);
}

void Server::exchangeData(Peer *peer)
{
  connect(peer->conn(), &QTcpSocket::connected, this, [this, peer]() {
    Message msg(MessageType::DataExchange);
    msg.setMetaData(m_self->name(), m_self->port(), m_self->pubKeyStr());
    peer->sendMsg(msg, false);
  });

  connect(peer->conn(), &QTcpSocket::readyRead, this, [this, peer]() {
    Message req(peer->conn()->readAll());

    if (req.type() == MessageType::Reject)
      onRemoteRejected(peer);
    if (req.type() == MessageType::Accept)
      onRemoteAccepted(peer, req);
  });
}

void Server::tryReconnecting(Peer *peer)
{
  if (peer->isActive()) {
    emit peerAlreadyConnected(peer->id());
  } else {
    exchangeData(peer);
    peer->connectToHost();
  }
}

/**
 * Public Functions
 */

Server::Server(Self *user, SessionManager *sm, QObject *parent)
  : QTcpServer{parent}
  , m_session(sm)
  , m_self(user)
{
  listen(m_self->address(), m_self->port());
  connect(m_self, &Self::portChanged, this, &Server::restartServer);
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
  Message res(MessageType::Accept);
  res.setMetaData(m_self->name(), m_self->port(), m_self->pubKeyStr());
  peer->sendMsg(res, false);
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

  Peer *peer = new Peer(m_self, this);

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
  Peer *peer = new Peer(m_self, conn, this);

  connect(peer->conn(), &QTcpSocket::readyRead, this, [this, peer]() {
    Message req(peer->conn()->readAll());

    if (req.type() == MessageType::DataExchange) {
      peer->setName(req.hostName());
      peer->setPort(req.port());
      peer->setPublicKey(req.publicKey());

      Peer *sessPeer = m_session->getPeer(peer->id());

      if (sessPeer != nullptr) {
        if (sessPeer->isActive()) {
          peer->deleteLater();
          qWarning() << "Peer is already connected and online";
          return;
        }

        disconnect(peer->conn(), &QTcpSocket::readyRead, nullptr, nullptr);
        sessPeer->setConn(peer->conn());
        sessPeer->setPublicKey(peer->publicKey());
        Message res(MessageType::Accept);
        res.setMetaData(m_self->name(), m_self->port(), m_self->pubKeyStr());
        sessPeer->sendMsg(res, false);
        sessPeer->setupHandler();
        peer->deleteLater();
        return;
      }

      emit newConnection(peer);
    }
  });
}

void Server::restartServer() {
  close();
  listen(m_self->address(), m_self->port());
}
