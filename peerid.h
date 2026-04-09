#ifndef PEERID_H
#define PEERID_H

#include <QHostAddress>
#include <QJsonObject>
#include <QTcpSocket>
#include "self.h"

struct PeerId
{
  QHostAddress addr;
  int port;

  bool operator==(const PeerId &o) const { return addr == o.addr && port == o.port; }

  explicit PeerId() {}

  explicit PeerId(QHostAddress addr, int port)
    : addr(addr)
    , port(port)
  {}

  QJsonObject toJson() const
  {
    QJsonObject obj;
    obj[Self::SettingsKeys::Addr] = addr.toString();
    obj[Self::SettingsKeys::Port] = port;
    return obj;
  }

  static PeerId fromJson(QJsonValue json)
  {
    return PeerId(QHostAddress(json[Self::SettingsKeys::Addr].toString()), json["port"].toInt());
  }
};

inline uint qHash(const PeerId &key, uint seed = 0)
{
  return qHash(key.addr, seed) ^ qHash(key.port, seed);
}

#endif // PEERID_H
