#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QUdpSocket>
#include "ipv6addrresolver.h"

class Server : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *socket;
    IPv6AddrResolver *ipr;

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

public slots:
    void initUdpSocket();
    void readPendingDatagrams();
};

#endif // SERVER_H
