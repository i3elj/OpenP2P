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

void Server::setupPeer(QString addrs, int port)
{
    std::println("accepted: {} {}", addrs.toStdString(), port);
    Peer *peer = activePeers->value(PeerId{.addr = QHostAddress(addrs), .port = port});
    peer->setup();
}

void Server::rejectPeer(QString addrs, int port)
{
    std::println("rejected: {} {}", addrs.toStdString(), port);
    PeerId id = PeerId{.addr = QHostAddress(addrs), .port = port};
    Peer *peer = activePeers->value(id);
    activePeers->remove(id);
    peer->close();
    peer->deleteLater();
}

Server::~Server() {}
