#include "server.h"

Server::Server(QHash<PeerId, Peer *> *activePeers, QObject *parent)
	: QTcpServer{parent}
	, activePeers(activePeers)
	, ipr(new IPv6AddrResolver(this))
{
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

	Peer *peer = new Peer(conn, id, this);
	emit newConnection(peer);
}

void Server::setupPeer(Peer *peer)
{
	activePeers->insert(peer->id(), peer);
	peer->setup();

	connect(peer, &Peer::newMsg, this, &Server::newMsg);
}

void Server::rejectPeer(Peer *peer)
{
	peer->close();
	peer->deleteLater();
}

Server::~Server() {}
