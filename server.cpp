#include "server.h"
#include <QDebug>
#include <QNetworkDatagram>
#include <QThread>

Server::Server(QObject *parent)
    : QObject{parent}
{
}

void Server::initUdpSocket()
{
    socket = new QUdpSocket(this);
    ipr = new IPv6AddrResolver(this);
    QList<QHostAddress> ips = ipr->resolve();

    socket->bind(ips.first(), 7755);

    connect(socket, &QUdpSocket::readyRead, this, &Server::readPendingDatagrams);
}

void Server::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray msg = datagram.data();
        qDebug() << msg;
    }
}

Server::~Server() {}
