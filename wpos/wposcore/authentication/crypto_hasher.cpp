// file      :  crypto_hasher.cpp
// birth     :  2/21/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "crypto_hasher.h"
#include <QCryptographicHash>

QByteArray MD5CryptoHasher::hash(const QByteArray &text) const{
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(text);
    return hasher.result().toHex();
}

QString MD5CryptoHasher::hash(const QString& text) const{
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(text.toUtf8());
    return hasher.result().toHex();
}

//QByteArray Sha1Hasher::hash(const QByteArray& text) const{
//    return QCryptographicHash::hash(text, QCryptographicHash::Sha1);
//}

//QByteArray Sha256Hasher::hash(const QByteArray &text) const{
//    return QCryptographicHash::hash(text, QCryptographicHash::Sha256);
//}

//QByteArray Sha512Hasher::hash(const QByteArray& text) const{
//    return QCryptographicHash::hash(text, QCryptographicHash::Sha512);
//}
