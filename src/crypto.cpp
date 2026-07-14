#include "src/crypto.hpp"
#include <QDebug>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/decoder.h>
#include <openssl/encoder.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

Crypto::Crypto(QObject *parent) : QObject{parent} {}

EVP_PKEY *Crypto::generateKey() {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if (!ctx) {
    qWarning() << "Failed to create key generation context";
    EVP_PKEY_CTX_free(ctx);
    return nullptr;
  }

  if (EVP_PKEY_keygen_init(ctx) <= 0) {
    qWarning() << "Failed to init keygen";
    EVP_PKEY_CTX_free(ctx);
    return nullptr;
  }

  if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 4096) <= 0) {
    qWarning() << "Failed to set key size";
    EVP_PKEY_CTX_free(ctx);
    return nullptr;
  }

  EVP_PKEY *pkey = nullptr;

  if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
    qWarning() << "Failed to generate key";
    EVP_PKEY_CTX_free(ctx);
    return nullptr;
  }

  EVP_PKEY_CTX_free(ctx);
  return pkey;
}

QByteArray Crypto::encrypt(EVP_PKEY *pkey, QByteArray &data) {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_pkey(NULL, pkey, NULL);
  unsigned char *out;
  size_t outlen = 0;
  const unsigned char *in = reinterpret_cast<const unsigned char *>(data.constData());
  size_t inlen = data.size();

  if (!ctx) {
    qWarning() << "Couldn't create the context";
    return NULL;
  }

  if (EVP_PKEY_encrypt_init(ctx) <= 0) {
    qWarning() << "Couldn't init the context";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
    qWarning() << "Couldn't set the padding";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_encrypt(ctx, NULL, &outlen, in, inlen) <= 0) {
    qWarning() << "Couldn't determine the buffer length";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  out = static_cast<unsigned char *>(OPENSSL_malloc(outlen));

  if (!out) {
    qWarning() << "Couldn't allocate memory";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen) <= 0) {
    qWarning() << "Couldn't encrypt";
    OPENSSL_free(out);
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  QByteArray result(reinterpret_cast<const char *>(out), outlen);
  OPENSSL_free(out);
  EVP_PKEY_CTX_free(ctx);
  return result;
}

QByteArray Crypto::decrypt(EVP_PKEY *pkey, QByteArray &data) {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_pkey(NULL, pkey, NULL);
  unsigned char *out;
  size_t outlen = 0;
  const unsigned char *in = reinterpret_cast<const unsigned char *>(data.constData());
  size_t inlen = data.size();

  if (!ctx) {
    qWarning() << "Couldn't create the context";
    return NULL;
  }

  if (EVP_PKEY_decrypt_init(ctx) <= 0) {
    qWarning() << "Couldn't init the context";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
    qWarning() << "Couldn't set the padding";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_decrypt(ctx, NULL, &outlen, in, inlen) <= 0) {
    qWarning() << "Couldn't determine the buffer length";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  out = static_cast<unsigned char *>(OPENSSL_malloc(outlen));

  if (!out) {
    qWarning() << "Couldn't allocate memory";
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  if (EVP_PKEY_decrypt(ctx, out, &outlen, in, inlen) <= 0) {
    qWarning() << "Couldn't decrypt";
    OPENSSL_free(out);
    EVP_PKEY_CTX_free(ctx);
    return {};
  }

  QByteArray result(reinterpret_cast<const char *>(out), outlen);
  OPENSSL_free(out);
  EVP_PKEY_CTX_free(ctx);
  return result;
}

QString Crypto::keyToPEM(EVP_PKEY *key) const {
  BIO *bio = BIO_new(BIO_s_mem());

  if (!bio) {
    qWarning() << "Failed to init BIO";
    BIO_free(bio);
    return "";
  }

  OSSL_ENCODER_CTX *ctx = OSSL_ENCODER_CTX_new_for_pkey(key, EVP_PKEY_PUBLIC_KEY, "PEM", NULL, NULL);

  if (!ctx) {
    qWarning() << "Failed to init OSSL_ENCODER_CTX";
    OSSL_ENCODER_CTX_free(ctx);
    BIO_free(bio);
    return "";
  }

  if (!OSSL_ENCODER_to_bio(ctx, bio)) {
    qWarning() << "Failed to encode public key";
    BIO_free(bio);
    OSSL_ENCODER_CTX_free(ctx);
    return "";
  }

  BUF_MEM *mem = nullptr;
  BIO_get_mem_ptr(bio, &mem);
  QByteArray pem(mem->data, static_cast<int>(mem->length));

  BIO_free(bio);
  OSSL_ENCODER_CTX_free(ctx);
  return QString::fromLatin1(pem);
}

EVP_PKEY *Crypto::PEMtoKey(QString keyStr) const {
  QString escapedKeyStr = keyStr;
  escapedKeyStr.replace("\\n", "\n");
  QByteArray pemBytes = escapedKeyStr.toLatin1();
  BIO *bio = BIO_new_mem_buf(pemBytes.constData(), pemBytes.size());

  if (!bio) {
    qWarning() << "Crypto::PEMtoKey(): Couldn't create a BIO";
    return nullptr;
  }

  EVP_PKEY *key = nullptr;
  OSSL_DECODER_CTX *ctx
    = OSSL_DECODER_CTX_new_for_pkey(&key, "PEM", NULL, NULL, EVP_PKEY_PUBLIC_KEY, NULL, NULL);

  if (!ctx) {
    qWarning() << "Crypto::PEMtoKey(): Couldn't create a decoder context";
    BIO_free(bio);
    OSSL_DECODER_CTX_free(ctx);
    return nullptr;
  }

  if (!OSSL_DECODER_from_bio(ctx, bio)) {
    qWarning() << "Crypto::PEMtoKey():OSSL_DECODER_from_bio() failed";
    OSSL_DECODER_CTX_free(ctx);
    BIO_free(bio);
    return nullptr;
  }

  BIO_free(bio);
  OSSL_DECODER_CTX_free(ctx);
  return key;
}
