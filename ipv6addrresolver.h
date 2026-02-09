#ifndef IPV6ADDRRESOLVER_H
#define IPV6ADDRRESOLVER_H

#include <QHostAddress>
#include <QObject>
#include "typedefs.h"

class IPv6AddrResolver : public QObject
{
  Q_OBJECT

public:
  explicit IPv6AddrResolver(QObject *parent = nullptr);
  AddressList resolve();

signals:
  void finished(QStringList ips);
};

#endif // IPV6ADDRRESOLVER_H
