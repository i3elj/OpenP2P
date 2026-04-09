#include "gui/peerlistmodel.h"

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

bool PeerListModel::removePeer(Peer *peer)
{
  int index = m_peers.indexOf(peer);

  if (index < 0)
    return false;

  beginRemoveRows(QModelIndex(), index, index);
  m_peers.removeAt(index);
  endRemoveRows();
  return true;
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
