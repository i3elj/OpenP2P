#ifndef PEERLISTMODEL_H
#define PEERLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "src/peer.hpp"

class PeerListModel : public QAbstractListModel
{
  Q_OBJECT

private:
  QList<Peer *> m_peers;

public:
  enum Roles { PeerRole };

  explicit PeerListModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  void addPeer(Peer *peer);
  bool removePeer(Peer *peer);

  Q_INVOKABLE Peer* at(int index) const;
  Q_INVOKABLE bool contains(Peer *peer) const;

signals:
};

#endif // PEERLISTMODEL_H
