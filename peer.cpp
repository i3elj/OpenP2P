#include "peer.h"
#include <iostream>

Peer::Peer(QTcpSocket* conn, QObject* parent)
    : QObject{parent}
{
    this->conn = conn;
    this->conn->setParent(this);
    addr = conn->peerAddress();
    addrStr = addr.toString();
    port = conn->peerPort();
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
}
