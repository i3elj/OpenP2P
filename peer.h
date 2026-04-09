#ifndef PEER_H
#define PEER_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
#include "crypto.h"
#include "gui/chatlistmodel.h"
#include "message.h"
#include "self.h"

struct PeerId
{
  QHostAddress addr;
  int port;

  bool operator==(const PeerId &o) const { return addr == o.addr && port == o.port; }

  explicit PeerId() {}

  explicit PeerId(QHostAddress addr, int port)
    : addr(addr)
    , port(port)
  {}

  QJsonObject toJson() const
  {
    QJsonObject obj;
    obj[Self::SettingsKeys::Addr] = addr.toString();
    obj[Self::SettingsKeys::Port] = port;
    return obj;
  }

  static PeerId fromJson(QJsonValue json)
  {
    return PeerId(QHostAddress(json[Self::SettingsKeys::Addr].toString()), json["port"].toInt());
  }
};

inline uint qHash(const PeerId &key, uint seed = 0)
{
  return qHash(key.addr, seed) ^ qHash(key.port, seed);
}

class Peer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString addr READ addr CONSTANT)
  Q_PROPERTY(int port READ port CONSTANT)
  Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
  Q_PROPERTY(ChatListModel *chatModel READ chatModel CONSTANT)

private:
  PeerId m_id;
  QString m_name;
  QHostAddress m_addr;
  int m_port;
  QTcpSocket *m_conn;
  bool m_active;
  ChatListModel* m_chatModel;
  Self* m_user;
  EVP_PKEY *m_pubKey;
  Crypto m_crypto;

public:
  explicit Peer(Self *user, QTcpSocket *conn, QObject *parent = nullptr);
  explicit Peer(Self *user, QObject *parent = nullptr);
  ~Peer();

  PeerId id() const;
  QString name() const;
  QString addr() const;
  int port() const;
  QTcpSocket *conn() const;
  bool isActive() const;
  ChatListModel *chatModel() const;
  QString publicKey() const;

  void setName(QString n);
  bool setAddr(QString address);
  void setPort(int port);
  void setConn(QTcpSocket *conn = new QTcpSocket);
  void setPublicKey(QString key);

  void setupHandler();
  void close();

  bool setAddressAndPort(QString address, int port);
  void connectToHost();

  void sendMsg(Message msg, bool encrypt = false);
  void loadMessages(QList<Message> messages);

  Q_INVOKABLE void sendMsg(QString txt);
  Q_INVOKABLE void saveChat();

public slots:
  void handle();
  void activate();
  void deactivate();
  void setupConnection();

signals:
  void nameChanged();
  void activeChanged();
  void connectionChanged();
  void newMsg(Message msg);
  void msgSent(Message msg);
  void accepted(Peer *p);
  void rejected(Peer *p);
};

#endif // PEER_H
