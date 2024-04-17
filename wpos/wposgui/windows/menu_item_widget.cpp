/*****************************************************************
                    menu_item_widget.cpp  -  description
                     ----------------------------
    begin            : 2/29/2024
    copyright        : (C) 2024 by Gilles Bene for WireSens Inc.
    email            : gilles.bene@wiresens.com

                     MIT License Version 2.0
    Copyright (c) 2016-2024  WireSens Inc. All rights reserved.
*******************************************************************/

#include "menu_item_widget.h"
#include "application_display.h"

MenuItemWidget::MenuItemWidget(
    ApplicationDisplay* display,
    QWidget* view,
    QTreeWidgetItem *parent,
    int type):
    QTreeWidgetItem{parent, type},
    display{display},
    _view{view}
{
    connect(this, &MenuItemWidget::clicked, this, &MenuItemWidget::setCurrentWidget);
}

QWidget* MenuItemWidget::view() const{ return _view;}

void MenuItemWidget::setCurrentWidget(QTreeWidgetItem*){
//    display->setCurrentWidget(_view);
}
