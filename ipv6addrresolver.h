#ifndef IPV6ADDRRESOLVER_H
#define IPV6ADDRRESOLVER_H

#include <QObject>
#include <qhostaddress.h>

class IPv6AddrResolver : public QObject
{
    Q_OBJECT

public:
    explicit IPv6AddrResolver(QObject *parent = nullptr);
    QStringList resolve();

signals:
    void finished(QStringList ips);
};

#endif // IPV6ADDRRESOLVER_H
