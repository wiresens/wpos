/***************************************************************************
                          bslmenupage.h  -  description
                             -------------------
    begin                : Tue Sep 9 2003
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

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QFrame>
#include <QWidget>
#include <QList>
#include <QMap>

/**
  *@author Carlos Manzanedo Rueda
  */

class QLayout;

class MenuPage : public QFrame{
    Q_OBJECT

public:
    enum LayoutType
    {
        GRID = 0,
        VBOX,
        HBOX
    };

    explicit MenuPage(QWidget *parent, const QString& name);
    ~MenuPage() = default;

    void setLayoutType(LayoutType type);
    LayoutType currentLayoutType();
    void addWidget(QWidget *wdgt, QString name, int row=0, int col=0);

private:
    void layGrid();
    void layVertical();
    void layHorizontal();

    QLayout *page_layout{};
    QList<QWidget*> widgetList;
    QMap<QString, QWidget*> widgetDict;
    LayoutType layout_type;
};

#endif
