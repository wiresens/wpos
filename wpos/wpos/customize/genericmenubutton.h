/***************************************************************************
                          genericmenubutton.h  -  description
                             -------------------
    begin                : jul 23 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GENERICMENUBUTTON_H
#define GENERICMENUBUTTON_H


#include <QPushButton>

/**
*    @author Carlos Manzanedo
*
*    this is only a button wich emits a genericSignal whenever it's pressed.
*    it can be used as a virtual class to inherit from.
*    the other class to implement this kind of generic buttons is GenericDataButton
*    wich also add to the class the capability of sending data with the signal.
*
*    These two classes are designed to be used with the generic signal manager.
*    The wright way to do this is the following:
*    1) create the button with a generic signal name, making SURE the signal name is a string.
*
*    2) Register the QObject at the generic signal manager and the rest of the classes
*       will receive the signal via the generic signal manager
*/

#include <QList>

struct RMenuButtonData{
    QString signal_name;
    QString data;
};

class QString;
class XmlConfig;
class GenericMenuButton : public QPushButton  {
    Q_OBJECT
public:
    explicit GenericMenuButton(const QString& text, QWidget *parent=0, const QString& name = QString{});
    ~GenericMenuButton(){ delete xml; }

    void createRelationship(const QString& signal_name, const QString& _data);

public slots:
    void clearRelationships();

protected slots:
    virtual void releasedSlot();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);

protected:
    //     void init();
    QList<RMenuButtonData*> *r_list;
    XmlConfig *xml;
};

#endif
