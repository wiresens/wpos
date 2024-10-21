/***************************************************************************
                          bslordertotalwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#ifndef ORDERFOOTERVIEW_H
#define ORDERFOOTERVIEW_H

#include <QFrame>

class QLabel;
class QHBoxLayout;
class XmlConfig;

class OrderFooterView : public QFrame  {
    Q_OBJECT
public:
    OrderFooterView(XmlConfig &footerXmlDesc, QWidget *parent, const QString& name);

public slots:
    void setTotal(float amount);

private:
    void parseXmlDescription(XmlConfig &footerXmlDesc);

private:
    QLabel* amountLabel{};
    QLabel* currencyLabel{};
    QHBoxLayout *layout{};
};

#endif
