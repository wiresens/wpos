/***************************************************************************
                          bslenterthematrix.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda
 ***************************************************************************/

#ifndef PASSWORD_CHANGE_DIALOG_H
#define PASSWORD_CHANGE_DIALOG_H
#include <QDialog>

namespace Ui {
class PasswordChangeDialog;
} // namespace Ui

struct Passwords;
class CryptoHasher;

class PasswordChangeDialog:
    public QDialog
{
    Q_OBJECT

public:
    explicit PasswordChangeDialog(const CryptoHasher& crypto, QWidget* parent = nullptr);
    ~PasswordChangeDialog(){ delete ui; }

public:
    Passwords readPasswords();

protected:
    virtual QString currentPassword() const;
    virtual QString newPassword() const;
    virtual QString confirmedPassword() const;
    virtual void clearPasswords();

protected slots:
    virtual void reject();
    void toggleButtonState();

private:
    Passwords passwords();

protected:
    Ui::PasswordChangeDialog* ui{};
    const CryptoHasher& hasher;
};

#endif //PASSWORD_CHANGE_DIALOG_H
