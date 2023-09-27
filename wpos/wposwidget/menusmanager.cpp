/***************************************************************************
                          bslmenusmanager.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "menusmanager.h"
//#include "prodselect/productscreenstack.h"
#include <QFrame>
#include <QLayout>
#include <iostream>

namespace std{}
using namespace std;

MenusManager::MenusManager(QWidget *parent, const QString &name ) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setObjectName(name);
}

MenusManager::~MenusManager(){}

void MenusManager::addMenu(const QString &name, QWidget *w){
    menusList.append(w);
    menusDict.insert(name,w);
    menuStack->addWidget(w);
    menuStack->setCurrentWidget(w);
}

QWidget *MenusManager::getCurrentMenu(){
    return menuStack->currentWidget();
}

void MenusManager::setMenu(QWidget *w){
    menuStack->setCurrentWidget(w);
}

void MenusManager::setMenu(int id){
    menuStack->setCurrentIndex(id);
}

void MenusManager::setMenu(QString name){
    QWidget *w = 0;
    w = menusDict.value(name);
    setMenu(w);
}

void MenusManager::setEnabled(bool enabled){
    QWidget::setEnabled(enabled);
}

void MenusManager::showScreenSlot(const QString &menu, const QString &subMenu){
    // Selects main menu
    setMenu(menu);
    if(subMenu.isEmpty()) return;
    // Select secondary menu
    emit showScreen(subMenu);
}
