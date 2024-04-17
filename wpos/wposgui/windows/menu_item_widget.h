/*****************************************************************
                    menu_item_widget.h  -  description
                     ----------------------------
    begin            : 2/29/2024
    copyright        : (C) 2024 by Gilles Bene for WireSens Inc.
    email            : gilles.bene@wiresens.com

                     MIT License Version 2.0
    Copyright (c) 2016-2024  WireSens Inc. All rights reserved.
*******************************************************************/

#ifndef MENU_ITEM_WIDGET_H
#define MENU_ITEM_WIDGET_H

#include <QTreeWidgetItem>
class ApplicationDisplay;

class MenuItemWidget : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    using QTreeWidgetItem::QTreeWidgetItem;
    MenuItemWidget(
        ApplicationDisplay* display,
        QWidget* view,
        QTreeWidgetItem *parent,
        int type = Type);

    QWidget* view() const;

signals:
    void clicked(QTreeWidgetItem *);

private slots:
    void setCurrentWidget(QTreeWidgetItem*);

private:
    QWidget* _view{};
    ApplicationDisplay* display;
};

#endif // MENU_ITEM_WIDGET_H
