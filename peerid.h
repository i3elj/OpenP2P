#ifndef PEERID_H
#define PEERID_H

#include <QHostAddress>
#include <QJsonObject>
#include <QTcpSocket>

struct PeerId
{
  QHostAddress addr;
  int port;

  bool operator==(const PeerId &o) const { return addr == o.addr && port == o.port; }

  explicit PeerId() {}

  explicit PeerId(QTcpSocket *conn)
  {
    addr = conn->peerAddress();
    port = conn->peerPort();
  }

  explicit PeerId(QHostAddress addr, int port)
    : addr(addr)
    , port(port)
  {}

  QJsonObject toJson() const
  {
    QJsonObject obj;
    obj["addr"] = addr.toString();
    obj["port"] = port;
    return obj;
  }

  static PeerId fromJson(QJsonValue json)
  {
    return PeerId(QHostAddress(json["addr"].toString()), json["port"].toInt());
  }
};

inline uint qHash(const PeerId &key, uint seed = 0)
{
  return qHash(key.addr, seed) ^ qHash(key.port, seed);
}

#endif // PEERID_H
