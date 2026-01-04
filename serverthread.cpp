#include "serverthread.h"
#include <qdebug.h>

ServerThread::ServerThread(QObject *parent)
    : QObject{parent}
{
    thread = new QThread(this);
    server = new Server();
    server->moveToThread(thread);

    connect(thread, &QThread::started, server, &Server::initUdpSocket);
    connect(thread, &QThread::finished, server, &Server::deleteLater);
}

void ServerThread::start()
{
    thread->start();
}

ServerThread::~ServerThread()
{
    if (thread && thread->isRunning()) {
        thread->quit();
        thread->wait();
    }
}
