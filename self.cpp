#include "self.h"
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>
#include "peer.h"

QString Self::userConfigDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
QString Self::savedPeersFilePath = Self::userConfigDir + "saved_peers.json";

Self::Self(QObject *parent)
  : QObject{parent}
  , m_name("")
  , m_settings(new QSettings(this))
{}

QString Self::name() const
{
  return m_name == "" ? m_settings->value("name", "").toString() : m_name;
}

void Self::setName(QString newName)
{
  if (m_name != newName) {
    m_name = newName;
    m_settings->setValue("name", m_name);
    emit nameChanged();
  }
}

int Self::port() const
{
  return m_port == 0 ? m_settings->value("port", 0).toInt() : m_port;
}

void Self::setPort(int port)
{
  if (m_port != port) {
    m_port = port;
    m_settings->setValue("port", m_port);
    emit portChanged();
  }
}
