#ifndef SERVER_H
#define SERVER_H

#include "ipv6addrresolver.h"
#include "peer.h"
#include "peerid.h"
#include <QTcpServer>
#include <QThread>

class Server : public QTcpServer {
	Q_OBJECT

private:
	IPv6AddrResolver *ipr;
	QHash<PeerId, Peer *> *activePeers;

public:
	Server(QHash<PeerId, Peer *> *activePeers, QObject *parent = nullptr);
	~Server();

public slots:
	void initTcpSocket();
	void handleNewConnection();
	void setupPeer(Peer *peer);
	void rejectPeer(Peer *peer);

signals:
	void newConnection(Peer *peer);
	void newMsg(Peer *from, QString msg);
};

#endif // SERVER_H
