#include "self.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>
#include "peer.h"

QString Self::userConfigDir;
QString Self::savedPeersFilePath;

void Self::createFiles()
{
  QDir dir(userConfigDir);
  QFile file(savedPeersFilePath);

  if (!dir.exists()) {
    dir.mkpath(userConfigDir);
  }

  if (file.open(QIODevice::Append)) {
    file.close();
  } else {
    qWarning() << "Couldn't create files:" << file.errorString();
  }
}

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
