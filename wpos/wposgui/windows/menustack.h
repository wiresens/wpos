/***************************************************************************
                          bslmenustack.h  -  description
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

#ifndef MENUSTACK_H
#define MENUSTACK_H

#include <QStackedWidget>
#include <QMap>
#include <QLayout>

class XmlConfig;
class MenuPage;

class MenuStack : public QStackedWidget  {
	Q_OBJECT
public:
    explicit MenuStack(
        QWidget *parent=nullptr,
        const QString &name =QString{});

    void addPage(QWidget *page, const QString &name);
    QWidget *currentPage();
    int numPages();

public slots:
    void setCurrentPage(const QString &name);
    void setCurrentPage(int pageIndex);

    void genericDataSignalSlot(const QString& signal_name, XmlConfig *xml);
    void genericSignalSlot(const QString& signal_name);

private :
    QMap<QString, QWidget*> pages;
};

#endif
