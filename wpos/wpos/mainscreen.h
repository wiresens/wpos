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

#include <wposgui/windows/menustack.h>

class XmlConfig;
class Greeter;
class SalesScreen;
class GenericSignalManager;
class ReadConfigScreen;
class QSplashScreen;

class MainScreen : public MenuStack{
    Q_OBJECT
public: 

    static const QString DBusService;
    static const QString LOGIN_SCREEN;
    static const QString SALES_SCREEN;
    static const QString CONFIG_SCREEN;

    explicit MainScreen(
        QSplashScreen &splash,
        const QString& name = QString{});

public slots:
    void genericSignalSlot(const QString& signal);
    void genericDataSignalSlot(
        const QString& signal,
        XmlConfig* xml);
    
private:
    Greeter          *m_login_screen{};
    SalesScreen      *m_sales_screen{};
    ReadConfigScreen *m_config_screen{};
};

#endif
