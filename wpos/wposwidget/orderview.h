/***************************************************************************
                          bslorderwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#ifndef ORDERVIEW_H
#define ORDERVIEW_H

#include <QFrame>

class OrderHeaderView;
class OrderContentView;
class OrderFooterView;
class QBoxLayout;
class XmlConfig;

class OrderView : public QFrame  {
    Q_OBJECT
public:
    explicit OrderView(QWidget *parent, const QString& name = QString());

    OrderContentView *orderContent();
    OrderHeaderView *orderHeader();

signals:
    void dataChanged(XmlConfig *xml);
    
public slots:
    void changeData(XmlConfig *xml);
    
protected:
    void parseXmlDescription(XmlConfig* xml);

    // Holds photo and the name of the waiter
    OrderHeaderView *order_header;
    // Holds information relative to the products, quantity, name, price, etc.
    OrderContentView *order_content;
    // Hols the total and the currency
    OrderFooterView *order_total;
    QBoxLayout *layout;
};

#endif
