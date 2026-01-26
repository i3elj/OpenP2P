#include "server.h"
#include <QDebug>
#include <QNetworkDatagram>
#include <QTcpSocket>
#include "peer.h"
#include <print>

Server::Server(QHash<PeerId, Peer *> *activePeers, QObject *parent)
    : QTcpServer{parent}
{
    this->activePeers = activePeers;
    this->ipr = new IPv6AddrResolver(this);

    QList<QHostAddress> addrs = ipr->resolve();
    listen(addrs.first(), 7755);
}

void Server::initTcpSocket()
{
    connect(this, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

void Server::handleNewConnection()
{
    QTcpSocket *conn = nextPendingConnection();
    PeerId id{.addr = conn->peerAddress(), .port = conn->peerPort()};

    if (activePeers->contains(id)) {
        conn->close();
        conn->deleteLater();
        return;
    }

    Peer *peer = new Peer(conn, this);
    activePeers->insert(id, peer);
    emit newConnection(peer);
}

void Server::setupPeer(Peer *peer)
{
    std::println("accepted: {} {}", peer->addrStr.toStdString(), peer->port);
    peer->setup();
}

void Server::rejectPeer(Peer *peer)
{
    std::println("rejected: {} {}", peer->addrStr.toStdString(), peer->port);
    PeerId id = PeerId{.addr = peer->addr, .port = peer->port};
    activePeers->remove(id);
    peer->close();
    peer->deleteLater();
}

Server::~Server() {}
