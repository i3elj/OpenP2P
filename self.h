#ifndef SELF_H
#define SELF_H

#include <QHostAddress>
#include <QObject>
#include <QSettings>
#include "crypto.h"
#include "ipv6addrresolver.h"

class Self : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

private:
  QSettings *m_settings;
  IPv6AddrResolver* m_ipr;
  QString m_name;
  QHostAddress m_address;
  int m_port;
  EVP_PKEY *m_pkey;
  Crypto m_crypto;

public:
  struct SettingsKeys {
    static constexpr const char *Type = "type";
    static constexpr const char *Text = "text";
    static constexpr const char *Sent = "sent";
    static constexpr const char *Name = "name";
    static constexpr const char *Addr = "addr";
    static constexpr const char *Port = "port";
    static constexpr const char *PublicKey = "public_key";
  };

  static QString userConfigDir;
  static QString savedPeersFilePath;
  static void createFiles();

  explicit Self(QObject *parent = nullptr);
  ~Self();
  QString name() const;
  QHostAddress address() const;
  int port() const;
  const EVP_PKEY *pubKey() const;
  QString pubKeyStr() const;

  void setName(QString newName);
  void setPort(int port);

  QByteArray encrypt(EVP_PKEY *pubKey, QByteArray &data);
  QByteArray decrypt(QByteArray &data);

signals:
  void nameChanged();
  void portChanged();
};

#endif // SELF_H
