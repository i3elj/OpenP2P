#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QAbstractItemModel>
#include <QObject>
#include "peer.h"

class SessionManager : public QObject
{
  Q_OBJECT

  public:
  explicit SessionManager(QObject *parent = nullptr);
  Q_INVOKABLE void saveChat(Peer *peer, QAbstractListModel *msgModel);

  signals:
};

#endif // SESSIONMANAGER_H
