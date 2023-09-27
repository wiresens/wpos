/***************************************************************************
                          bslgenericscreenwidget.h  -  description
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

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeWidget>
class QTreeWidgetItem;

class TreeView : public QTreeWidget  {
    Q_OBJECT
public:
    explicit TreeView(QWidget* parent=nullptr);
    ~TreeView();

signals:
    void setMenuSignal(QString name); //@benes nodeClicked(QString name)

public slots:
    void nodeClicked(QTreeWidgetItem *item); //@benes setMenu(QTreeWidgetItem *item)

};

#endif
