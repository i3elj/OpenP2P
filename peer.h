#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include "peerid.h"

class Peer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString addrStr MEMBER addrStr CONSTANT)
    Q_PROPERTY(int port MEMBER port CONSTANT)

public:
    PeerId id;
    QHostAddress addr;
    QString addrStr;
    int port;
    QTcpSocket* conn;

    explicit Peer(QTcpSocket* conn, PeerId id, QObject* parent = nullptr);
    void setup();
    void close();
    void disconnected();

public slots:
    void handle();

signals:
    void newMsg(Peer* peer);
};

#endif // PEER_H
