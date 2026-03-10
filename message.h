#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

class Message
{
public:
  enum Type { Reject = 0, Accept = 1, Common = 2, DataExchange = 3 };

  explicit Message();
  explicit Message(Type t);
  explicit Message(QString name);
  explicit Message(bool sent, QString msg);
  explicit Message(QByteArray data);

  QJsonObject toJson();
  QByteArray toBytes();
  void setMetaData(QString name, int port);

  QString text();
  QString hostName() const;
  int Port() const;
  Message::Type type() const;

private:
  QString m_text;
  bool m_sent;
  QString m_hostName;
  int m_port;
  Message::Type m_type;
};

#endif // MESSAGE_H
