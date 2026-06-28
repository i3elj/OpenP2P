#include "message.h"
#include "self.h"

Message::Message()
  : m_text("")
  , m_sent(false)
  , m_hostName("")
  , m_port(0)
  , m_type(MessageType::Common)
{}

Message::Message(MessageType t)
  : m_text("")
  , m_sent(false)
  , m_hostName("")
  , m_port(0)
  , m_type(t)
{}

Message::Message(QString name)
  : m_text("")
  , m_sent(false)
  , m_hostName(name)
  , m_port(0)
  , m_type(MessageType::Common)
{}

Message::Message(bool sent, QString msg)
  : m_text(msg)
  , m_sent(sent)
  , m_hostName("")
  , m_port(0)
  , m_type(MessageType::Common)
{}

Message::Message(QByteArray data) {
  QJsonObject json(QJsonDocument::fromJson(data).object());
  m_type = MessageType(json[Self::SettingsKeys::Type].toInt());

  switch (m_type) {
  case Common:
    m_text = json[Self::SettingsKeys::Text].toString();
    m_sent = json[Self::SettingsKeys::Sent].toBool();
    break;
  case Accept:
  case DataExchange:
    m_hostName = json[Self::SettingsKeys::Name].toString();
    m_port = json[Self::SettingsKeys::Port].toInt();
    m_publicKey = json[Self::SettingsKeys::PublicKey].toString();
    break;
  default:
    break;
  }
}

QJsonObject Message::toJson() {
  QJsonObject json{{Self::SettingsKeys::Type, m_type}};

  switch (m_type) {
  case Common:
    json[Self::SettingsKeys::Text] = m_text;
    json[Self::SettingsKeys::Sent] = m_sent;
    break;

  case DataExchange:
  case Accept:
    json[Self::SettingsKeys::Name] = m_hostName;
    json[Self::SettingsKeys::Port] = m_port;
    json[Self::SettingsKeys::PublicKey] = m_publicKey;
    break;

  default:
    break;
  };

  return json;
}

QByteArray Message::toBytes() {
  return QJsonDocument(toJson()).toJson(QJsonDocument::Compact);
}

void Message::setMetaData(QString name, int port, QString publicKey) {
  m_hostName = name;
  m_port = port;
  m_publicKey = publicKey;
}

bool Message::sent() const { return m_sent; }

QString Message::text() const { return m_text; }

QString Message::hostName() const { return m_hostName; }

int Message::port() const { return m_port; }

MessageType Message::type() const { return m_type; }

QString Message::publicKey() const { return m_publicKey; }

void Message::setText(QString txt) { m_text = txt; }

void Message::setSent(bool sent) { m_sent = sent; }

void Message::setHostName(QString name) { m_hostName = name; }

void Message::setPort(int port) { m_port = port; }

void Message::setType(MessageType type) { m_type = type; }
