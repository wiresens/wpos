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

    OrderHeaderView *headerView() const { return m_header ;}
    OrderContentView *contentView() const { return m_content ;}
    OrderFooterView *footerView() const { return m_footer ;}

signals:
    void orderChanged(XmlConfig *xml);
    
public slots:
    void updateOrder(XmlConfig *xml);
    
private:
    void parseXmlDescription(XmlConfig& orderXmlDesc);

private:
    OrderHeaderView *m_header; // waiter's photo and the name, table name
    OrderContentView *m_content; //product quantity, name, price, etc.
    OrderFooterView *m_footer; // total and the currency
    QBoxLayout *layout;
};

#endif
