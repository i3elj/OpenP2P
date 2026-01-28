#include "peer.h"
#include <iostream>

Peer::Peer(QTcpSocket* conn, PeerId id, QObject* parent)
    : QObject{parent}
    , id(id)
    , conn(conn)
{
    this->conn->setParent(this);
    addr = this->conn->peerAddress();
    addrStr = addr.toString();
    port = this->conn->peerPort();
}

void Peer::setup()
{
    connect(conn, &QTcpSocket::readyRead, this, &Peer::handle);
}

void Peer::close()
{
    conn->write("Rejected");
    conn->abort();
}

void Peer::handle()
{
    QByteArray data = conn->readAll();
    std::cout << std::format("{}", data.toStdString());
    emit newMsg(this);
}
