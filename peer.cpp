#include "peer.h"
#include <iostream>

Peer::Peer(QTcpSocket *conn, PeerId id, QObject *parent)
	: QObject{parent}, m_id(id), m_conn(conn) {
	m_conn->setParent(this);
	m_addr = m_conn->peerAddress();
	m_port = m_conn->peerPort();
}

void Peer::setup() {
	connect(m_conn, &QTcpSocket::readyRead, this, &Peer::handle);
}

void Peer::close() {
	m_conn->write("Rejected");
	m_conn->abort();
}

PeerId Peer::id() const
{
	return PeerId{.addr = m_addr, .port = m_port};
}

QString Peer::addr() const { return m_addr.toString(); }

int Peer::port() const { return m_port; }

QString Peer::name() const { return m_name; }

void Peer::setName(QString n) {
	if (m_name == n)
		return;
	m_name = n;
	emit nameChanged();
}

void Peer::handle() {
	QByteArray data = m_conn->readAll();
	std::cout << std::format("{}", data.toStdString());
	emit newMsg(this, QString::fromStdString(data.toStdString()));
}
