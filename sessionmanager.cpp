#include "sessionmanager.h"

SessionManager::SessionManager(QObject *parent) : QObject{parent} {}

void SessionManager::saveChat(Peer *peer, QAbstractItemModel *msgs) {
	auto roles = msgs->roleNames();
	int sentRole = roles.key("sent");
	int textRole = roles.key("text");

	for (int i = 0; i < msgs->rowCount(); ++i) {
			QModelIndex idx = msgs->index(i, 0);
			bool sent = msgs->data(idx, sentRole).toBool();
			QString text = msgs->data(idx, textRole).toString();

			if (sent) {
				qInfo() << "I said:" << text << "to" << peer->name();
			} else {
				qInfo() << peer->name() << "said:" << text;
			}
	}
}
