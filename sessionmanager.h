#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "peer.h"
#include <QObject>
#include <QAbstractItemModel>

class SessionManager : public QObject {
	Q_OBJECT

public:
	explicit SessionManager(QObject *parent = nullptr);
	Q_INVOKABLE void saveChat(Peer *peer, QAbstractItemModel *msgs);

signals:
};

#endif // SESSIONMANAGER_H
