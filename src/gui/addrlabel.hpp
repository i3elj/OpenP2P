#ifndef ADDRLABEL_H
#define ADDRLABEL_H

#include <QObject>
#include <QThread>
#include "src/ipv6addrresolver.hpp"

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
