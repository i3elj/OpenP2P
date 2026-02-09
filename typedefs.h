#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QHostAddress>
#include "peer.h"
#include "peerid.h"

typedef QList<QHostAddress> AddressList;
typedef QHash<PeerId, Peer *> PeerHashMap;

#endif // TYPEDEFS_H
