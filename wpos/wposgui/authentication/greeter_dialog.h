// file      :  greeter_dialog.h
// birth     :  2/10/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef GREETER_DIALOG_H
#define GREETER_DIALOG_H

#include <wposcore/authentication/credential.h>
#include <wposmodel/src/core/user.h>
#include <wposmodel/src/core/session_manager.h>

#include <QDialog>

using wpos::model::UserPtr;
using wpos::SessionManager;

namespace Ui {
class GreeterDialog;
} // namespace Ui

struct Credential;
class CryptoHasher;
class CredentialManager;

class GreeterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GreeterDialog(QWidget* parent = nullptr);

    ~GreeterDialog();

public:
    bool authenticate();
    PasswordCredential readCredential();

protected:
    virtual UserPtr authenticate(const Credential& cred);
    virtual void notifyFailed(const QString& msg);
    void prompt();


    virtual QString getLogin() const;
    virtual QString getPassword() const;

protected slots:
    virtual void reject();
    virtual void accept() final;

private slots:
    void toggleConnectState();
    void read();

private:
    PasswordCredential credentials();
    void clearCredentials();

private:
    Ui::GreeterDialog* ui{};
    SessionManager* sessionManager{};
    QPushButton* connectBtn{};
    QPushButton* resetBtn{};
    QPushButton* cancelBtn{};
};

#endif //GREETER_DIALOG_H
