// file      :  credential.h
// birth     :  2/21/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <QString>

class QString;

struct Credential{
    virtual ~Credential(){}
    virtual const QString& id() const = 0;  //login
    virtual const QString& key() const = 0;  //password
};

class PasswordCredential : public Credential
{
public:
    PasswordCredential(
        const QString& login,
        const QString& password);

    PasswordCredential(PasswordCredential&&) = default;
    PasswordCredential& operator=(PasswordCredential&&) = default;

    virtual const QString& id() const override;  //login
    virtual const QString& key() const override;  //password

private:
    const QString login;
    const QString password;
};

struct Passwords{
    const QString currentPass;
    const QString newPass;
    const QString confirmPass;
};

#endif // CREDENTIAL_H
