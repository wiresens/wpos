// file      :  credential.cpp
// birth     :  2/21/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "credential.h"
#include "crypto_hasher.h"

static MD5CryptoHasher hasher;

PasswordCredential::PasswordCredential(
    const QString &login,
    const QString &password)
    : login{login},
    password{hasher.hash(password)}
{}

const QString& PasswordCredential::id()const
{
    return login;
}

const QString& PasswordCredential::key()const
{
    return password;
}





