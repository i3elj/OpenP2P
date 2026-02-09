#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QAbstractItemModel>
#include <QObject>
#include "message.h"
#include "peer.h"
#include "typedefs.h"

class SessionManager : public QObject
{
  Q_OBJECT

private:
  const QString m_chatsFilePath = "/OpenP2P/chats";
  PeerHashMap m_activePeers;

public:
  explicit SessionManager(QObject *parent = nullptr);
  void addPeer(PeerId id, Peer *peer);

  Q_INVOKABLE void saveChat(Peer *peer, QAbstractListModel *msgModel);
  Q_INVOKABLE QList<Message> loadChat(Peer *peer);

signals:
};

#endif // SESSIONMANAGER_H
