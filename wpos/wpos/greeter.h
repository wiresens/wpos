/***************************************************************************
                          bslenterthematrix.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

/**
        CLASS documentation

        -At the init it takes the users from the db and generate buttons.

        -Buttons activate the communication with the Dcopfx2000 process and check the identity

        -if the identity is confirmed the dcopfx2000 process will send us a confirmation with
        the quality and similarity of the fingerprint.

        -if the confirm message does not arrives it makes nothing, pressing the buttons does only
        tries to get a new confirmation message

        -if the confirm message arrives the widget sends the GDATASIGNAL_CHANGE_USER whith the
        confirmed user

        -At every new show the widget check with the database if the list of generated buttons is
        the same as the list of users at the database... if there are more they are created, if there
        are less they are deleted.
*/

#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include "ui_loginwidget.h"
#include "greeterinterface.h"
#include <wposcore/hlist.h>

class AuthCoreDB;
class QPushButton;
class XmlConfig;
class MenuPage;

class Greeter :
        public QWidget,
        public GreeterInterface,
        private Ui::LoginWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wpos.Greeter")

public:
    static const QString DBusObject ;
    static constexpr QSize LOGIN_BUTTON_SIZE = QSize{100,70};

    Greeter(MenuPage* parent, const QString& name);
    ~Greeter();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);

public slots:
    void registerUser(const QString& user_id);
    void genericSignalSlot(const QString& signal_name);

    Q_SCRIPTABLE void getMatchResults(QString _xml_match_data);

protected slots:
    void handleButtonPressed();
    void flushFingerprintPixmap();

protected:
    void showEvent(QShowEvent *e);
    void createLoginButtons();

    //   void keyPressEvent(QKeyEvent *e);
private:
    void loadPicture();

private:
    //if state is true, the auth will be used, if false
    //no auth will be done at all.
    bool use_auth{false};
};

#endif
