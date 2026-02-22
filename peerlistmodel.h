#ifndef PEERLISTMODEL_H
#define PEERLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "typedefs.h"

class PeerListModel : public QAbstractListModel
{
  Q_OBJECT

private:
  QList<Peer *> m_peers;

public:
  enum Roles { NameRole = Qt::UserRole + 1, AddrRole, PortRole };
  explicit PeerListModel(QObject *parent = nullptr);
  void setPeers(const PeerHashMap &peers);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  void addPeer(Peer *peer);
  void removePeer(Peer *peer);

  Q_INVOKABLE Peer* at(int index);

public slots:
  void peerDataChanged(Peer *peer);

signals:
};

#endif // PEERLISTMODEL_H
