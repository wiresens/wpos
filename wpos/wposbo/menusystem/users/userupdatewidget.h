/***************************************************************************
                          bslupdateuser.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef USER_UPDATE_WIDGET_H
#define USER_UPDATE_WIDGET_H

#include "usereditionwidget.h"

class UserUpdateWidget : public UserEditionWidget  {
    Q_OBJECT
public:
    explicit UserUpdateWidget(UserModule *_user_mod, QWidget *parent=0, const QString& name = QString());

    //try to update the user at the current update user widget.
    bool updateUser();

public slots:
    //set the user with the employee id given at the string
    void setUser(QString user_id);

protected:
    virtual void doEnroll() override;
    virtual void doActualize() override;
    virtual void doFinalize() override;
};

#endif // USER_UPDATE_WIDGET_H
