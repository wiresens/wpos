/***************************************************************************
                          bslmenupage.cpp  -  description
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

#include "menupage.h"
#include <wposwidget/global.h>
#include <QLayout>

MenuPage::MenuPage(QWidget *parent, const QString name):
    QFrame(parent)
{
    setObjectName(name);
    page_layout = nullptr;

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(QColor(Colors::MENU_PAGE_BG_COLOR)));
    setBackgroundRole(QPalette::Window);
}

MenuPage::~MenuPage(){}

void MenuPage::setLayoutType(LAYOUT_TYPE lay){

    switch(lay){
    case GRID:
        setGridLayoutAsCurrent();
        break;
    case VBOX:
        setVBoxLayoutAsCurrent();
        break;
    case HBOX:
        setHBoxLayoutAsCurrent();
        break;
    }
    page_layout->setContentsMargins(0,0,0,5);
}

MenuPage::LAYOUT_TYPE MenuPage::currentLayoutType(){
    return layout_type;
}

void MenuPage::addWidget(QWidget *w, QString name){

    widgetList.append(w);
    widgetDict.insert(name,w);

    switch(layout_type){
    case GRID:
        // Not applicable to this layout
        break;
    case VBOX:
        ((QBoxLayout *)page_layout)->addWidget(w);
        break;
    case HBOX:
        ((QBoxLayout *)page_layout)->addWidget(w);
        break;
    }
}

void MenuPage::addWidget(QWidget *w, QString name, int index){

    widgetList.append(w);
    widgetDict.insert(name,w);

    switch(layout_type){
    case GRID:
        // Not applicable to this layout
        break;
    case VBOX:
        ((QBoxLayout *)page_layout)->insertWidget(index, w);
        break;
    case HBOX:
        ((QBoxLayout *)page_layout)->insertWidget(index, w);
    }
}

void MenuPage::addWidget(QWidget *w, QString name, int row, int col){

    widgetList.append(w);
    widgetDict.insert(name,w);

    switch(layout_type){
    case GRID:
        ((QGridLayout *)page_layout)->addWidget(w,row,col);
        break;
    case VBOX:
        // Not applicable to this layout
        break;
    case HBOX:
        // Not applicable to this layout
        break;
    }
}

void MenuPage::setGridLayoutAsCurrent(){

    layout_type = GRID;
    delete page_layout;
    page_layout = new QGridLayout(this);
}

void MenuPage::setVBoxLayoutAsCurrent(){

    layout_type = VBOX;
    delete page_layout;
    page_layout = new QVBoxLayout(this);
}

void MenuPage::setHBoxLayoutAsCurrent(){

    layout_type = HBOX;
    delete page_layout;
    page_layout = new QHBoxLayout(this);
}
