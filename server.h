#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include "ipv6addrresolver.h"
#include "peer.h"
#include "sessionmanager.h"

class Server : public QTcpServer {
	Q_OBJECT

private:
  IPv6AddrResolver *m_ipr;
  SessionManager *m_session;

public:
  Server(SessionManager *sm, QObject *parent = nullptr);
  ~Server();

public slots:
	void initTcpSocket();
	void handleNewConnection();
	void setupPeer(Peer *peer);
	void rejectPeer(Peer *peer);

signals:
	void newConnection(Peer *peer);
	void newMsg(Peer *from, QString msg);
};

#endif // SERVER_H
