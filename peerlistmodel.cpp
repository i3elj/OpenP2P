#include "peerlistmodel.h"

PeerListModel::PeerListModel(QObject *parent)
  : QAbstractListModel(parent)
  , m_peers()
{}

void PeerListModel::addPeer(Peer *peer)
{
  int listSize = m_peers.size();
  beginInsertRows(QModelIndex(), listSize, listSize);
  m_peers.append(peer);
  endInsertRows();
}

void PeerListModel::removePeer(Peer *peer)
{
  beginRemoveRows(QModelIndex(), m_peers.size(), m_peers.size());
  int index = m_peers.indexOf(peer);
  m_peers.removeAt(index);
  endRemoveRows();
}

int PeerListModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;
  return m_peers.size();
}

QVariant PeerListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() >= m_peers.size())
    return {};

  Peer *peer = m_peers.at(index.row());

  return QVariant::fromValue(peer);
}

QHash<int, QByteArray> PeerListModel::roleNames() const
{
  return {{PeerRole, "peer"}};
}

Peer *PeerListModel::at(int index) const
{
  return m_peers.at(index);
}

bool PeerListModel::contains(Peer *peer) const
{
  return m_peers.contains(peer);
}
