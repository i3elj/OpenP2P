#ifndef PEERID_H
#define PEERID_H

#include <QHostAddress>
#include <QTcpSocket>

struct PeerId
{
    QHostAddress addr;
    int port;

    bool operator==(const PeerId &o) const { return addr == o.addr && port == o.port; }

    explicit PeerId(QTcpSocket *conn)
    {
      addr = conn->peerAddress();
      port = conn->peerPort();
    }
};

inline uint qHash(const PeerId &key, uint seed = 0)
{
    return qHash(key.addr, seed) ^ qHash(key.port, seed);
}

#endif // PEERID_H
