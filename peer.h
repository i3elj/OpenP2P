#ifndef PEER_H
#define PEER_H

#include "peerid.h"
#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>

class Peer : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString addr READ addr CONSTANT)
	Q_PROPERTY(int port READ port CONSTANT)

private:
	PeerId m_id;
	QTcpSocket *m_conn;
	QHostAddress m_addr;
	int m_port;
	QString m_name;


public:
	explicit Peer(QTcpSocket *conn, PeerId id, QObject *parent = nullptr);
	void setup();
	void close();
	PeerId id() const;
	QString addr() const;
	int port() const;
	QString name() const;
	void setName(QString n);

public slots:
	void handle();

signals:
	void nameChanged();
	void newMsg(Peer *from, QString msg);
};

#endif // PEER_H
