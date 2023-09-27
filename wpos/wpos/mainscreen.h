/***************************************************************************
                          bsllinuxbar.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <wposwidget/menustack.h>

class XmlConfig;
class LoginScreen;
class SalesScreen;
class GenericSignalManager;
class ReadConfigScreen;
class QSplashScreen;

class MainScreen : public MenuStack  {
    Q_OBJECT
public: 

    static const QString DBusService;
    static const QString LOGIN_SCREEN;
    static const QString SALES_SCREEN;
    static const QString READ_CONFIG_SCREEN;

    explicit MainScreen(QSplashScreen &splash_screen, const QString& name = QString());

public slots:
    void genericSignalSlot(const QString& signal);
    void genericDataSignalSlot(const QString& signal, XmlConfig *xml);
    
private:
    LoginScreen *login_screen;
    SalesScreen *sales_screen;
    ReadConfigScreen *config_screen;
};

#endif
