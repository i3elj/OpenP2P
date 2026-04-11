#include "self.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>
#include <openssl/evp.h>
#include <openssl/rsa.h>

QString Self::userConfigDir;
QString Self::savedPeersFilePath;

void Self::createFiles() {
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
  , m_ipr(new IPv6AddrResolver(this))
  , m_name(m_settings->value(SettingsKeys::Name, "").toString())
  , m_address(m_ipr->resolve().first())
  , m_port(m_settings->value(SettingsKeys::Port, 7755).toInt())
  , m_pkey(nullptr)
  , m_crypto(this)
{
  m_pkey = m_crypto.generateKey();
}

Self::~Self() { EVP_PKEY_free(m_pkey); }

QString Self::name() const {
  return m_settings->value(SettingsKeys::Name, m_name).toString();
}

QHostAddress Self::address() const { return m_address; }

int Self::port() const {
  return m_settings->value(SettingsKeys::Port, m_port).toInt();
}

const EVP_PKEY *Self::pubKey() const { return m_pkey; }

QString Self::pubKeyStr() const { return m_crypto.keyToPEM(m_pkey); }

void Self::setName(QString newName) {
  if (m_name != newName) {
    m_name = newName;
    m_settings->setValue(SettingsKeys::Name, m_name);
    emit nameChanged();
  }
}

void Self::setPort(int port) {
  if (m_port != port) {
    m_port = port;
    m_settings->setValue(SettingsKeys::Port, m_port);
    emit portChanged();
  }
}

QByteArray Self::encrypt(EVP_PKEY *pubKey, QByteArray &data) {
  return m_crypto.encrypt(pubKey, data);
}

QByteArray Self::decrypt(QByteArray &data) {
  return m_crypto.decrypt(m_pkey, data);
}
