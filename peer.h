#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>

class Peer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString addrStr MEMBER addrStr CONSTANT)
    Q_PROPERTY(int port MEMBER port CONSTANT)

public:
    QHostAddress addr;
    QString addrStr;
    int port;
    QTcpSocket* conn;

    explicit Peer(QTcpSocket* conn, QObject* parent = nullptr);
    void setup();
    void close();
    void disconnected();

public slots:
    void handle();
};

#endif // PEER_H
