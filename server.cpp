#include "server.h"
#include <QDebug>
#include <QNetworkDatagram>
#include <QThread>

Server::Server(QObject *parent)
    : QObject{parent}
{
    thread = new QThread;

    moveToThread(thread);
    connect(thread, &QThread::started, this, &Server::initUdpSocket);

    this->thread->start();
}

void Server::initUdpSocket()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 7755);

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

Server::~Server()
{
    if (thread->isRunning()) {
        thread->quit();
        thread->wait();
    }
}
