#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QHostAddress>
#include "src/peer.hpp"

typedef QList<QHostAddress> AddressList;
typedef QHash<PeerId, Peer *> PeerHashMap;

#endif // TYPEDEFS_H
