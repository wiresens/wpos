// file      :  credential_manager.h
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef CREDENTIAL_MANAGER_H
#define CREDENTIAL_MANAGER_H

#include <QObject>

class BasicMainWindow;
class GreeterDialog;
struct Credential;

#include <QSessionManager>

class CredentialManager : public QObject
{
    Q_OBJECT
public:
    explicit CredentialManager(BasicMainWindow *managed);

    virtual void logIn();

signals:

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    BasicMainWindow* managedWindow{};
    GreeterDialog* greeter{};

};

#endif // CREDENTIAL_MANAGER_H
