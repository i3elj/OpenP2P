#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include "server.h"

class ServerThread : public QObject
{
    Q_OBJECT
    QThread *thread;
    Server *server;

public:
    ServerThread(QObject *parent = nullptr);
    ~ServerThread();
    void start();

signals:
};

#endif // SERVERTHREAD_H
