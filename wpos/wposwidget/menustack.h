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
#include <QWidget>
#include <QList>
#include <QMap>
#include <QLayout>

#include <wposcore/genericsignalmanager.h>

class XmlConfig;
class MenuPage;

class MenuStack : public QStackedWidget  {
	Q_OBJECT
public:
    explicit MenuStack(QWidget *parent=nullptr, const QString &name =QString());
    ~MenuStack();

	virtual void addPage(QWidget *page, QString name);
	virtual QWidget *currentPage();
	virtual bool isMenuPage();
	virtual bool isPage();
	virtual int numPages();

public slots:
	virtual void setCurrentPage(QString name);
	virtual void setCurrentPage(int pageIndex);

    virtual void genericDataSignalSlot(const QString& signal_name, XmlConfig *xml);
	virtual void genericSignalSlot(const QString& signal_name);

protected  :
    HList<QWidget> pages;
};

#endif
