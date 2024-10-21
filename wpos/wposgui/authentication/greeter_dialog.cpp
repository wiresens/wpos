// file      :  greeter_dialog.cpp
// birth     :  2/10/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "ui_greeter_dialog.h"
#include "authentication/greeter_dialog.h"
#include "widget/caption_widget.h"

#include <QPushButton>
#include <database.h>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <string>
#include <QDebug>

using std::string;

using namespace wpos::model;
static const QString& connecText{QObject::tr("Connecter")};
static const QString& clearText{QObject::tr("Effacer")};
static const QString& cancelText{QObject::tr("Annuler")};
static const QString& failiureMsg{QObject::tr("Echec de connection! Vérifiez vos paramètres et rééssayez")};

GreeterDialog::GreeterDialog(
    QWidget* parent)
    :QDialog(parent),
    ui{new Ui::GreeterDialog()},
    sessionManager{new SessionManager()}
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GreeterDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &GreeterDialog::reject);

    ui->infoFrame->clearAll();
    ui->infoFrame->hide();
    ui->login_label->hide();
    ui->password_label->hide();

    ui->login->setFocus();
    connectBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    resetBtn = ui->buttonBox->button(QDialogButtonBox::Reset);
    cancelBtn = ui->buttonBox->button(QDialogButtonBox::Cancel);

    connectBtn->setText(connecText);
    resetBtn->setText(clearText);
    cancelBtn->setText(cancelText);

    connectBtn->setEnabled(false);
    resetBtn->setEnabled(false);

    setWindowFlag(Qt::FramelessWindowHint);

    connect(ui->login, &QLineEdit::textChanged, this, &GreeterDialog::toggleConnectState );
    connect(ui->password, &QLineEdit::textChanged, this, &GreeterDialog::toggleConnectState );
    connect(resetBtn, &QPushButton::clicked, this, &GreeterDialog::clearCredentials);
}

GreeterDialog::~GreeterDialog(){
    delete ui;
    delete sessionManager;
}

bool GreeterDialog::authenticate(){
    prompt();
    return result();
}

UserPtr GreeterDialog::authenticate(const Credential& cred){
    odb::database& db{ wpos::database::instance()};

    odb::transaction transaction(db.begin());
        auto user_ptr =  sessionManager->authenticate(
            cred.id().toStdString(),
            cred.key().toStdString()
        );
    transaction.commit();
    return user_ptr;
}

void GreeterDialog::prompt(){
    exec();
}

PasswordCredential GreeterDialog::readCredential(){
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GreeterDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GreeterDialog::read);
    connectBtn->setText(connecText);
        prompt();
    connectBtn->setText(clearText);
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GreeterDialog::read);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GreeterDialog::accept);

    return credentials();
}

void GreeterDialog::notifyFailed(const QString& msg){
    ui->infoFrame->setCaption(msg);
    ui->infoFrame->setVisible(true);
}

QString GreeterDialog::getLogin() const{
    return ui->login->text();
}

QString GreeterDialog::getPassword() const{
    return ui->password->text();
}

void GreeterDialog::reject(){
    QDialog::reject();
}

void GreeterDialog::accept(){
    auto cred = credentials();
    auto user = authenticate(cred);
    if (user)
        QDialog::accept();
    else
        notifyFailed(failiureMsg);
}

void GreeterDialog::toggleConnectState(){
    auto password = getPassword();
    auto login = getLogin();
    connectBtn->setEnabled( !login.isEmpty() && !password.isEmpty());
    resetBtn->setDisabled( login.isEmpty() && password.isEmpty());
}

void GreeterDialog::read(){
    QDialog::accept();
}

void GreeterDialog::clearCredentials(){
    ui->login->clear();
    ui->password->clear();
    ui->infoFrame->setVisible(false);
    toggleConnectState();
}

PasswordCredential GreeterDialog::credentials(){
    auto cred = PasswordCredential{ getLogin(), getPassword()};
    qDebug() << cred.id() <<":"<< cred.key();
    return PasswordCredential{ getLogin(), getPassword()};
}

