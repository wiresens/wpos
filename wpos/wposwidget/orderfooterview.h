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

#include <wposcore/hlist.h>
#include <QFrame>

class QLabel;
class QHBoxLayout;
class XmlConfig;

class OrderFooterView : public QFrame  {
    Q_OBJECT
public:
    OrderFooterView(XmlConfig *xml, QWidget *parent, const QString& name);

public slots:
    void totalBillSlot(float amount);

protected:
    void parseXmlDescription(XmlConfig *xml);
    HList<QLabel> labels;
    QHBoxLayout *layout{};
};

#endif
