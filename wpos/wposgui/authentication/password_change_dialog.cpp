/***************************************************************************
                          bslenterthematrix.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "ui_password_change_dialog.h"
#include "password_change_dialog.h"
#include <wposcore/authentication/credential.h>
#include <wposcore/authentication/crypto_hasher.h>

#include <QPushButton>

PasswordChangeDialog::PasswordChangeDialog(
    const CryptoHasher &crypto,
    QWidget *parent):
    QDialog(parent), hasher{crypto}
{
    ui = new Ui::PasswordChangeDialog();
    ui->setupUi(this);
    ui->new_pass_label->setFocus();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Change");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->currentPassword, &QLineEdit::textEdited, this, &PasswordChangeDialog::toggleButtonState );
    connect(ui->newPassword, &QLineEdit::textEdited, this, &PasswordChangeDialog::toggleButtonState );
    connect(ui->confirmPassword, &QLineEdit::textEdited, this, &PasswordChangeDialog::toggleButtonState );
}

Passwords PasswordChangeDialog::readPasswords(){
    exec();
    return passwords();
}

QString PasswordChangeDialog::currentPassword() const{
    return hasher.hash(ui->currentPassword->text());
}

QString PasswordChangeDialog::newPassword() const{
    return hasher.hash(ui->newPassword->text());
}

QString PasswordChangeDialog::confirmedPassword() const{
    return hasher.hash(ui->confirmPassword->text());
}

void PasswordChangeDialog::reject(){
    clearPasswords();
    QDialog::reject();
}

void PasswordChangeDialog::toggleButtonState(){
    auto connectBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    Passwords passwd = passwords();
    connectBtn->setEnabled( !passwd.currentPass.isEmpty() &&
                           !passwd.newPass.isEmpty() &&
                           !passwd.confirmPass.isEmpty());
}

void PasswordChangeDialog::clearPasswords(){
    ui->currentPassword->clear();
    ui->newPassword->clear();
    ui->confirmPassword->clear();
}

Passwords PasswordChangeDialog::passwords(){
    return Passwords{ currentPassword(), newPassword(), confirmedPassword()};
}
