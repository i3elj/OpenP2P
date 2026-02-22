#include "peerlistmodel.h"
#include "typedefs.h"

PeerListModel::PeerListModel(QObject *parent) : QAbstractListModel(parent) {}

void PeerListModel::setPeers(const PeerHashMap &peers) {
  // todo
}

void PeerListModel::addPeer(Peer *peer) {
  int listSize = m_peers.size();
  beginInsertRows(QModelIndex(), listSize, listSize);
  m_peers.append(peer);
  endInsertRows();

  connect(peer, &Peer::nameChanged, this,
          [this, peer]() { peerDataChanged(peer); });
}

void PeerListModel::removePeer(Peer *peer) {
  beginRemoveRows(QModelIndex(), m_peers.size(), m_peers.size());
  int index = m_peers.indexOf(peer);
  m_peers.removeAt(index);
  endRemoveRows();
}

void PeerListModel::peerDataChanged(Peer *peer) {
  int row = m_peers.indexOf(peer);

  if (row < 0)
    return;

  QModelIndex i = index(row);
  emit dataChanged(i, i, {NameRole});
}

int PeerListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_peers.size();
}

QVariant PeerListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= m_peers.size())
    return {};

  Peer *peer = m_peers.at(index.row());

  switch (role) {
  case NameRole:
    return peer->name();
  case AddrRole:
    return peer->addr();
  case PortRole:
    return peer->port();
  };

  return {};
}

QHash<int, QByteArray> PeerListModel::roleNames() const {
  return {{NameRole, "name"}, {AddrRole, "addr"}, {PortRole, "port"}};
}

Peer *PeerListModel::at(int index)
{
  return m_peers.at(index);
}
