#ifndef PEERID_H
#define PEERID_H

#include <QHostAddress>

struct PeerId
{
    QHostAddress addr;
    int port;

    bool operator==(const PeerId &o) const { return addr == o.addr && port == o.port; }
};

inline uint qHash(const PeerId &key, uint seed = 0)
{
    return qHash(key.addr, seed) ^ qHash(key.port, seed);
}

#endif // PEERID_H
