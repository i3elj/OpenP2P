#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include "ipv6addrresolver.h"
#include "peer.h"
#include "peerid.h"

class Server : public QTcpServer
{
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
    void setupPeer(QString addrs, int port);
    void rejectPeer(QString addrs, int port);

signals:
    void newConnection(Peer *peer);
};

#endif // SERVER_H
