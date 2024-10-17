/***************************************************************************
                          bslmenustack.cpp  -  description
                             -------------------
    begin                : Mon Jul 7 2003
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "menustack.h"
#include "common/global.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/genericsignalmanager.h>

#include <QColor>

MenuStack::MenuStack(QWidget *parent, const QString& name) :
    QStackedWidget(parent)
{
    setObjectName(name);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(QColor(Colors::MENU_STACK_BG_COLOR)));
    setBackgroundRole(QPalette::Window);

    //gsignal attach signals
    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericSignal(GSIGNAL::ENABLE_MAINSTACK, this);
    gsm->subscribeToGenericSignal(GSIGNAL::DISABLE_MAINSTACK, this);
}

int MenuStack::numPages(){
    return count();
}

QWidget* MenuStack::currentPage(){
    return currentWidget();
}

void MenuStack::addPage(
    QWidget *page,
    const QString &name)
{
    pages[name] = page;
    addWidget(page);
    setCurrentWidget(page);
    setEnabled(true);
}

void MenuStack::setCurrentPage(const QString &name){
    auto* wgt = pages.value(name);
    if(wgt){
        setCurrentWidget(wgt);
        setEnabled(true);
    }
}

void MenuStack::setCurrentPage(int pageIndex){
    if( pageIndex >= 0 && pageIndex < count() ){
        setCurrentIndex(pageIndex);
        setEnabled(true);
    }
}

void MenuStack::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::ENABLE_MAINSTACK ){
        setEnabled(true);
    }
    else if (signal_name == GSIGNAL::DISABLE_MAINSTACK ){
        setEnabled(false);
    }
}

void MenuStack::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){

    if (signal_name == GDATASIGNAL::MAINSTACK_SET_PAGE){
        xml->pushDomain();
        xml->delDomain();
        setCurrentPage(xml->readString("name"));
        xml->popDomain();
    }
}
