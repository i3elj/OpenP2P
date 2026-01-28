#ifndef ADDRLABEL_H
#define ADDRLABEL_H

#include <QObject>
#include <QThread>
#include "ipv6addrresolver.h"

class AddrLabel : public QObject
{
    Q_OBJECT

private:
    IPv6AddrResolver *ipr;
    QThread *thread;

public:
    explicit AddrLabel(QObject *parent = nullptr);
    ~AddrLabel();

signals:
    void ipsReceived(QStringList ips);
};

#endif // ADDRLABEL_H
