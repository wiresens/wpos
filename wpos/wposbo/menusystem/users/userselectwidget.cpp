/***************************************************************************
                          bslselectuserswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "userselectwidget.h"
#include "userupdatewidget.h"

#include "usersmodule/usermodule.h"
#include "databasemodule/databasemodule.h"

#include <libbslxml/xmlconfig.h>

#include <QMessageBox>

static const QString& USERS_LIST_DTD  {"dtddocs:users_userslist.dtd"};
//Ui Messages:
static const QString& HARD_DELETE_BUTTON_TEXT = QObject::tr("Permanently\nDelete");
static const QString& DB_USER_CHANGED_TITLE = QObject::tr("Change of user in the database");
static const QString& DB_USER_CHANGED_MSG = QObject::tr("There has been a change in the users part of the database...\nThe database is going to be read again");
static const QString& DB_CONFIG_CHANGED_TITLE = QObject::tr("Change of configuration in the database");
static const QString& DB_CONFIG_CHANGED_MSG = QObject::tr("There has been a change in the database configuration...\nThe database will be reread");
static const QString& FAILED_TO_DELETE_USER = QObject::tr("Failed to delete user");
static const QString& FAILED_TO_UPDATE_USER_TITLE = QObject::tr("failed to update user");
static const QString& INFO_NO_USER_SELECTED_MSG = QObject::tr("No user selected\n");
static const QString& INFO_DELETING_USER_TITLE = QObject::tr("Deleting user %1");
static const QString& INFO_DELETING_USER_MSG = QObject::tr("The user with DNI %1 and name %2 will be completely eliminated from the system.\n Contrary to the deletion operation, this operation\n will eliminate the statistics, fingerprints and personal data of said user.\nAre you sure you want to proceed with this operation??");
static const QString& ERROR_DELETING_USER_TITLE = QObject::tr("Error deleting user %1");
static const QString& ERROR_DELETING_USER_MSG = QObject::tr("It has not been possible to delete the user %1 \nwith DNI %2 from the database \n\n If the waiter has tickets in progress, he cannot be deleted \nhe will have to do a Z to be able to do this operation");

UserSelectWidget::UserSelectWidget(DatabaseModule *_db_mod, UserModule *_user_mod, int _mode,
                                   QWidget *parent, const QString& name) :
    QWidget(parent), user_mod{_user_mod},
    db_mod{_db_mod}, state{(State)_mode}
{
    struct NullPtrDatabaseModuleException{};
    struct NullPtrUserModuleException{};

    if ( !user_mod ) throw NullPtrUserModuleException{};
    if (!db_mod) throw NullPtrDatabaseModuleException{};

    setupUi(this);
    setObjectName(name);

    users_view->setSelectionMode(QAbstractItemView::SingleSelection);
    users_view->setColumnWidth(0,40);
    users_view->setColumnWidth(1,150);
    users_view->setColumnWidth(2,500);

    cancel_button->setIcon(QPixmap("controls48:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));

    if (state == State::Updater){
        real_del_button->hide();
        update_user = new UserUpdateWidget(user_mod, action_page, "modificar");

        QLayout *layout;
        if ( !(layout = action_page->layout()) )
            layout = new QHBoxLayout(action_page);

        layout->addWidget(update_user);
        connect(users_view, &QTreeWidget::itemDoubleClicked, this, &UserSelectWidget::userSelected);
    }
    else{
        real_del_button->setIcon(QPixmap("controls48:trash.png"));
        real_del_button->setText(HARD_DELETE_BUTTON_TEXT);
        connect(real_del_button, &QPushButton::clicked, this, &UserSelectWidget::realDelSlot);
    }

    connect(ok_button, &QPushButton::clicked, this, &UserSelectWidget::accept);
    connect(cancel_button, &QPushButton::clicked, this, &UserSelectWidget::cancel);
}

void UserSelectWidget::cancel(){
    users_view->clearSelection();
    stack->setCurrentWidget(select_user_page);
    //what is going to be done when the cancel buttons are pressed ??????
    //clear also the other widgets...
}

void UserSelectWidget::accept(){
    QTreeWidgetItem *item {};
    QString user_id, name, text;

    switch (state){
    case State::Deleter:    //the delete user section
        if ( !(item = users_view->currentItem()) ){
            //no item selected... advice it.
            text = INFO_NO_USER_SELECTED_MSG;
            QMessageBox::warning(this, FAILED_TO_DELETE_USER, text);
            return;
        }
        user_id = item->text(UserId);
        name = item->text(UserName);
        text = INFO_DELETING_USER_MSG.arg(user_id).arg(name);
        if ( QMessageBox::question(this, INFO_DELETING_USER_TITLE.arg(name), text, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel )
            users_view->clearSelection();
        else{
            //disconnect the userChangedSlot()
            // disconnect(user_mod, SIGNAL(updatesAvailable()),this,SLOT(userChangedSlot()));
            if (!user_mod->deleteUser(user_id)){
                text = ERROR_DELETING_USER_MSG;
                QMessageBox::critical(this, ERROR_DELETING_USER_TITLE.arg(name), text);
            }
            loadUsers();
            //reconnect the userChangedSlot()
            // connect(user_mod, SIGNAL(updatesAvailable()), this, SLOT(userChangedSlot()));
        }

    case State::Updater: //The update user section
        if ( stack->currentWidget() == action_page){
            if (update_user->updateUser()) stack->setCurrentWidget(select_user_page);
        }
        else{
            if ( !(item = users_view->currentItem()) ){
                //no item selected... advice it.
                text = INFO_NO_USER_SELECTED_MSG;
                QMessageBox::warning(this, FAILED_TO_UPDATE_USER_TITLE, text);
                return;
            }
            user_id = item->text(UserId);
            stack->setCurrentWidget(action_page);
            update_user->setUser(user_id);
        }
        break;
    }
}

void UserSelectWidget::userSelected(QTreeWidgetItem* item, int colum){
    if( ! item) return;
    stack->setCurrentWidget(action_page);
    update_user->setUser(item->text(UserId));
}

void UserSelectWidget::reloadUsers(){
    QString text = DB_USER_CHANGED_MSG;
    QMessageBox::information(this, DB_USER_CHANGED_TITLE, text);
    stack->setCurrentWidget(select_user_page);
    loadUsers();
}

void UserSelectWidget::refreshUsersPage(QString name){
    if ( state == State::Updater && stack->currentWidget() == action_page )
        if ( update_user->userId() == name )
            reloadUsers();
}

void UserSelectWidget::databaseChangedSlot(){
    QString text = DB_CONFIG_CHANGED_MSG;
    QMessageBox::information(this, DB_CONFIG_CHANGED_TITLE, text);
    stack->setCurrentWidget(select_user_page);
    loadUsers();
}

void UserSelectWidget::loadUsers(){
    QString users_xml = user_mod->usersList();
    if ( users_xml.isEmpty() ) return;

    XmlConfig xml;
    xml.readXmlFromString(users_xml);
    if ( !xml.wellFormed() || !xml.validateXmlWithDTD(USERS_LIST_DTD, true)) return;

    users_view->clear();
    QTreeWidgetItem *current_item {};
    QTreeWidgetItem *preceding_item {};
    xml.delDomain();
    xml.setDomain("users");
    QString user_id, user_name, user_last_name;
    for ( auto i=0; i < xml.howManyTags("user"); i++){
        user_id = xml.readString("user["+QString::number(i)+"].employee_id");
        user_name = xml.readString("user["+QString::number(i)+"].name");
        user_last_name = xml.readString("user["+QString::number(i)+"].last_name");

        //do not show the system user.
        if (user_id == "1") continue;

        if ( i == 0 ) current_item = new QTreeWidgetItem(users_view);
        else current_item = new QTreeWidgetItem(users_view, preceding_item);

        current_item->setText(Number, QString::number(i));
        current_item->setText(UserId, user_id);
        current_item->setText(UserName, user_name+" "+user_last_name);
        preceding_item = current_item;
    }
    users_view->clearSelection();
    xml.debug();
}

void UserSelectWidget::showEvent(QShowEvent *e){
    connect(user_mod, &UserModule::updatesAvailable, this, &UserSelectWidget::reloadUsers);
    connect(db_mod, &DatabaseModule::configChanged, this, &UserSelectWidget::databaseChangedSlot);
    stack->setCurrentWidget(select_user_page);
    loadUsers();
    QWidget::showEvent(e);
}

void UserSelectWidget::hideEvent(QHideEvent *e){
    disconnect(user_mod, &UserModule::updatesAvailable, this, &UserSelectWidget::reloadUsers);
    disconnect(db_mod, &DatabaseModule::configChanged, this, &UserSelectWidget::databaseChangedSlot);
    QWidget::hideEvent(e);
}

void UserSelectWidget::realDelSlot(){

    QTreeWidgetItem *item;
    if (!(item = users_view->currentItem())){
        auto text = INFO_NO_USER_SELECTED_MSG;
        QMessageBox::warning(this, FAILED_TO_DELETE_USER, text);
        return;
    }

    auto employee_id = item->text(UserId);
    auto name = item->text(UserName);
    auto text = INFO_DELETING_USER_MSG.arg(employee_id).arg(name);

    switch (QMessageBox::question(this, INFO_DELETING_USER_TITLE.arg(name), text, QMessageBox::Ok, QMessageBox::Cancel))
    {
    case QMessageBox::Cancel:
        users_view->clearSelection();
        break;
    case QMessageBox::Ok:
        if ( !user_mod->realDelUser(employee_id) ){
            text = ERROR_DELETING_USER_MSG.arg(name).arg(employee_id);
            QMessageBox::critical(this, ERROR_DELETING_USER_TITLE.arg(name), text);
        }
        loadUsers();
    }
}
