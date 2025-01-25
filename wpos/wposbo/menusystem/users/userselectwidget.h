/***************************************************************************
                          bslselectuserswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLSELECTUSERSWIDGET_H
#define BSLSELECTUSERSWIDGET_H

#include "ui_userselectwidgetbase.h"

class UserModule;
class DatabaseModule;
class UserUpdateWidget;

class UserSelectWidget : public QWidget, private Ui::UserSelectWidgetBase {
    Q_OBJECT
public:
    enum State {
        Deleter = 0,
        Updater = 1
    };

    enum Column {
        Number = 0,
        UserId = 1,
        UserName = 2
    };

    UserSelectWidget(
        DatabaseModule* _db_mod,
        UserModule* _user_mod, int state,
        QWidget* parent = 0, const QString& name = QString {});

    ~UserSelectWidget() = default;

public slots:
    void cancel();
    void accept();
    void loadUsers();

    void reloadUsers();
    void refreshUsersPage(QString);
    void databaseChangedSlot();
    void realDelSlot();

protected slots:
    void userSelected(QTreeWidgetItem* item, int colum);

protected:
    void showEvent(QShowEvent* e);
    void hideEvent(QHideEvent* e);

    UserModule* user_mod {};
    DatabaseModule* db_mod {};
    UserUpdateWidget* update_user {};

    State state { Updater };
};

#endif
