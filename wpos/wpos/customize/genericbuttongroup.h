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
        const QString& name = QString{});

    QPushButton* buttonByName(const QString& name);

    bool readConfigFrom(const QString& section, const QString& xmlFile);
    void clear();

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

private:
    bool readConfigFrom(const QString& section, XmlConfig &xml);
    QPushButton* createButton(XmlConfig &xml, QWidget *parent, bool append=true);

    //generic types
    QPushButton* createProductButton(XmlConfig &xml, QWidget *parent);
    QPushButton* createGenericButton(XmlConfig &xml, QWidget *parent);
    QPushButton* createMenuButton(XmlConfig &xml, QWidget *parent);

    //special types (composed)
    QPushButton* createScreensButton(XmlConfig &xml, QWidget *parent);
    QPushButton* createOptionsButton(XmlConfig &xml, QWidget *parent);
    QPushButton* createDefaultScreenButton(XmlConfig &xml, QWidget *parent);

    void setSizeAndColorPolicy(QPushButton *button, XmlConfig &xml);

private:
    HList<QPushButton> m_buttons{};
    ProductScreenStack *m_product_screen_stack{};
    QWidget *m_parent{};
};

#endif
