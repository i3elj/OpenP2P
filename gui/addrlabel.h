#ifndef ADDRLABEL_H
#define ADDRLABEL_H

#include <QObject>
#include "ipv6addrresolver.h"

class AddrLabel : public QObject
{
    Q_OBJECT
    IPv6AddrResolver *ipr;

public:
    explicit AddrLabel(QObject *parent = nullptr);

signals:
    void ipReceived(QStringList ips);
};

#endif // ADDRLABEL_H
