/***************************************************************************
                          genericbuttongroup.h  -  description
                             -------------------
    begin                : vie may 7 2004
    copyright            : (C) 2004 by Carlos Manzanedo
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

#ifndef GENERIC_BUTTON_GROUP_H
#define GENERIC_BUTTON_GROUP_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <wposcore/hlist.h>
#include "prodselect/productscreenstack.h"

class QPushButton;
class XmlConfig;
class QString;

class GenericButtonGroup : public QObject  {
    Q_OBJECT

public:
    explicit GenericButtonGroup(
        ProductScreenStack *stack,
        QWidget *parent=0,
        const QString& name = QString());

    QPushButton* findButton(const QString& name);
    QPushButton* find(const QString& name);

public slots:
    void clear();
    bool readConfigFrom(const QString& section, const QString& xmlFile);
    bool readConfigFrom(const QString& section, XmlConfig *xml);

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected:
    QPushButton* readButtonConfig(XmlConfig *xml, QWidget *parent, bool append=true);

    //generic types
    QPushButton* readProductConfig(XmlConfig *xml, QWidget *parent);
    QPushButton* readGenericButton(XmlConfig *xml, QWidget *parent);
    QPushButton* readMenuButton(XmlConfig *xml, QWidget *parent);

    //special types (composed)
    QPushButton* screensButton(XmlConfig *xml, QWidget *parent);
    QPushButton* optionsButton(XmlConfig *xml, QWidget *parent);
    QPushButton* defaultScreenButton(XmlConfig *xml, QWidget *parent);

    void setSizeAndColorPolicy(QPushButton *button, XmlConfig *xml);

    HList<QPushButton> buttons{};
    ProductScreenStack *stack{};
    QWidget *parent{};
    QString sectionName;
};

#endif
