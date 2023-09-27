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

class MenuPage : public QFrame  {
    Q_OBJECT
public:

    enum LAYOUT_TYPE
    {
        GRID=0,
        VBOX,
        HBOX
    };

    explicit MenuPage(QWidget *parent, const QString name = QString());
    ~MenuPage();

    void setLayoutType(LAYOUT_TYPE lay);
    LAYOUT_TYPE currentLayoutType();
    void addWidget(QWidget *w, QString name);
    void addWidget(QWidget *w, QString name, int index);
    void addWidget(QWidget *w, QString name, int row, int col);

private:
    void setGridLayoutAsCurrent();
    void setVBoxLayoutAsCurrent();
    void setHBoxLayoutAsCurrent();

    LAYOUT_TYPE layout_type;
    QLayout *page_layout;
    QList<QWidget*> widgetList;
    QMap<QString, QWidget*> widgetDict;
};

#endif
