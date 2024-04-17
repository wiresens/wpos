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
#include "ui_menusmanager.h"
//#include "prodselect/productscreenstack.h"
#include <QFrame>
#include <QLayout>
#include <iostream>

namespace std{}
using namespace std;

MenusManager::MenusManager(QWidget *parent, const QString &name ) :
    QWidget(parent),
    ui {new Ui::MenusManager()}
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setObjectName(name);
}

MenusManager::~MenusManager(){
    delete ui;
}

void MenusManager::addMenu(const QString &name, QWidget *w){
    menusList.append(w);
    menusDict.insert(name,w);
    ui->menuStack->addWidget(w);
    ui->menuStack->setCurrentWidget(w);
}

QWidget *MenusManager::getCurrentMenu(){
    return ui->menuStack->currentWidget();
}

void MenusManager::setMenu(QWidget *w){
    ui->menuStack->setCurrentWidget(w);
}

void MenusManager::setMenu(int id){
    ui->menuStack->setCurrentIndex(id);
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
