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
#include "common/global.h"
#include <QLayout>

MenuPage::MenuPage(
    QWidget *parent,
    const QString &name)
    :QFrame(parent)
{
    setObjectName(name);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setPalette(QPalette(QColor(Colors::MENU_PAGE_BG_COLOR)));
    setBackgroundRole(QPalette::Window);
}

void MenuPage::setLayoutType(LayoutType type){
    switch(type){
    case LayoutType::GRID:
        layGrid();
        break;
    case LayoutType::VBOX:
        layVertical();
        break;
    case LayoutType::HBOX:
        layHorizontal();
        break;
    }
    page_layout->setContentsMargins(0,0,0,5);
}

MenuPage::LayoutType MenuPage::currentLayoutType(){
    return layout_type;
}

void MenuPage::addWidget(
    QWidget *wdgt,
    QString name,
    int row,
    int col)
{
    switch(layout_type){
    case LayoutType::GRID:
        ((QGridLayout *)page_layout)->addWidget(wdgt, row, col);
        break;
    case LayoutType::VBOX:
        ((QBoxLayout *)page_layout)->addWidget(wdgt);
        break;
    case LayoutType::HBOX:
        ((QBoxLayout *)page_layout)->addWidget(wdgt);
        break;
    }
}

void MenuPage::layGrid(){
    layout_type = LayoutType::GRID;
    delete page_layout;
    page_layout = new QGridLayout(this);
}

void MenuPage::layVertical(){
    layout_type = LayoutType::VBOX;
    delete page_layout;
    page_layout = new QVBoxLayout(this);
}

void MenuPage::layHorizontal(){
    layout_type = LayoutType::HBOX;
    delete page_layout;
    page_layout = new QHBoxLayout(this);
}
