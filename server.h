#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <QUdpSocket>

class Server : public QObject
{
    Q_OBJECT

private:
    QThread *thread;
    QUdpSocket *socket;

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    void init();

public slots:
    void initUdpSocket();
    void readPendingDatagrams();
};

#endif // SERVER_H
