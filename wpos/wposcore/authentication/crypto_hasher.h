// file      :  crypto_hasher.h
// birth     :  2/21/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef CRYPTO_HASHER_H
#define CRYPTO_HASHER_H

#include <QString>

class CryptoHasher{
public:
    virtual QByteArray hash(const QByteArray&) const = 0;
    virtual QString hash(const QString&)  const = 0;
};

class MD5CryptoHasher : public CryptoHasher{
public:
    virtual QByteArray hash(const QByteArray& text) const override;
    virtual QString hash(const QString& text) const override;
};

//class Sha1Hasher : public CryptoHasher{
//public:
//    virtual QByteArray hash(const QByteArray &text) const override;
//};

//class Sha256Hasher : public CryptoHasher{
//public:
//    virtual QByteArray hash(const QByteArray& text) const override;
//};

//class Sha512Hasher : public CryptoHasher{
//public:
//    virtual QByteArray hash(const QByteArray &text) const override;
//};

#endif // CRYPTO_HASHER_H
