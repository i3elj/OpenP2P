#include "self.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>
#include "peer.h"

QString Self::userConfigDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/OpenP2P/";
QString Self::savedPeersFilePath = Self::userConfigDir + "saved_peers.json";

Self::Self(QObject *parent)
  : QObject{parent}
  , m_settings(new QSettings(this))
  , m_name(m_settings->value("name", "").toString())
  , m_port(m_settings->value("port", 7755).toInt())
{}

QString Self::name() const
{
  return m_settings->value("name", m_name).toString();
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
  return m_settings->value("port", m_port).toInt();
}

void Self::setPort(int port)
{
  if (m_port != port) {
    m_port = port;
    m_settings->setValue("port", m_port);
    emit portChanged();
  }
}

void Self::createFiles() const
{
  QDir dir;
  QFile file(savedPeersFilePath);

  if (!dir.exists(userConfigDir)) {
    dir.mkdir(userConfigDir);
  }

  if (file.open(QIODevice::Append)) {
    file.close();
  }
}
