#include "message.h"
#include "self.h"

Message::Message() {}

Message::Message(Type t)
  : m_type(t)
{}

Message::Message(QString name)
  : m_hostName(name)
  , m_type(Type::DataExchange)
{}

Message::Message(bool sent, QString msg)
  : m_text(msg)
  , m_sent(sent)
  , m_type(Type::Common)
{}

Message::Message(QByteArray data)
{
  QJsonObject json(QJsonDocument::fromJson(data).object());
  m_type = Type(json[Self::SettingsKeys::Type].toInt());

  auto keys = json.keys();
  if (keys.contains(Self::SettingsKeys::Name) && keys.contains(Self::SettingsKeys::Port)) {
    m_hostName = json[Self::SettingsKeys::Name].toString();
    m_port = json[Self::SettingsKeys::Port].toInt();
  }

  if (m_type == Common) {
    m_text = json[Self::SettingsKeys::Text].toString();
    m_sent = json[Self::SettingsKeys::Sent].toBool();
  }
}

QJsonObject Message::toJson()
{
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
    break;

  default:
    break;
  };

  return json;
}

QByteArray Message::toBytes()
{
  return QJsonDocument(toJson()).toJson(QJsonDocument::Compact);
}

void Message::setMetaData(QString name, int port)
{
  m_hostName = name;
  m_port = port;
}

QString Message::text()
{
  return m_text;
}

QString Message::hostName() const
{
  return m_hostName;
}

int Message::Port() const
{
  return m_port;
}

Message::Type Message::type() const
{
  return m_type;
}
