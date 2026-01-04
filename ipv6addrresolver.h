#ifndef IPV6ADDRRESOLVER_H
#define IPV6ADDRRESOLVER_H

#include <QHostAddress>
#include <QObject>

class IPv6AddrResolver : public QObject
{
    Q_OBJECT

public:
    explicit IPv6AddrResolver(QObject *parent = nullptr);
    QList<QHostAddress> resolve();

signals:
    void finished(QStringList ips);
};

#endif // IPV6ADDRRESOLVER_H
