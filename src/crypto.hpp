#ifndef CRYPTO_H
#define CRYPTO_H
#include <QByteArray>
#include <QObject>
#include <openssl/evp.h>

class Crypto : public QObject {
  Q_OBJECT

public:
  explicit Crypto(QObject *parent = nullptr);

  EVP_PKEY *generateKey();
  QByteArray encrypt(EVP_PKEY *pkey, QByteArray &data);
  QByteArray decrypt(EVP_PKEY *pkey, QByteArray &data);

  QString keyToPEM(EVP_PKEY *key) const;
  EVP_PKEY *PEMtoKey(QString keyStr) const;
};

#endif // CRYPTO_H
