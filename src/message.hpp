#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

enum MessageType {
  Reject,
  Accept,
  Common,
  DataExchange,
  Update,
};

class Message
{
private:
  QString m_text;
  bool m_sent;
  QString m_hostName;
  int m_port;
  QString m_publicKey;
  MessageType m_type; 

public:

  explicit Message();
  explicit Message(MessageType t);
  explicit Message(QString name);
  explicit Message(bool sent, QString msg);
  explicit Message(QByteArray data);

  QJsonObject toJson();
  QByteArray toBytes();
  void setMetaData(QString name, int port, QString publicKey);

  QString text() const;
  bool sent() const;
  QString hostName() const;
  int port() const;
  MessageType type() const;
  QString publicKey() const;

  void setText(QString txt);
  void setSent(bool sent);
  void setHostName(QString name);
  void setPort(int port);
  void setType(MessageType type);
};

#endif // MESSAGE_H
