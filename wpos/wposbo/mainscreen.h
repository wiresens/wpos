/***************************************************************************
                          linuxbarbackoffice.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QWidget>

class QVBoxLayout;
class MenuStack;
class BslAccessScreenWidget;
class MainScreenMenu;

/******************************************************************************
 * This widget contains a menus_stack which has all pages.
 *
 * This works as follows:
 *
 *   + First page is authentification one. If we are recognized by system
 *      we pass to main_screen
 *
 *   + Second page is main_screen. From here we can select main
 *      actions by a categorized system.
 *
 *****************************************************************************/

/** LinuxBarBackOffice is the base class of the project */
class MainScreen : public QWidget{
    Q_OBJECT

public:
    explicit MainScreen(QWidget* parent=nullptr);
    ~MainScreen() = default;

protected:
    void setupApplication();
    void buildMenuStack();
    void buildScreenMenu();

    QVBoxLayout *mainLayout{};
    MenuStack   *menuStack{};
    MainScreenMenu  *screenMenu{};
};

#endif
