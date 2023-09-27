/***************************************************************************
                          paymodewidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PAYMODESWIDGET_H
#define PAYMODESWIDGET_H

#include <QWidget>
#include "ui_paymodeswidget.h"

class BarCore;
class QString;
class XmlConfig;
class QPushButton;

class PayModes : public QWidget, private Ui::PayModesWidget  {
    Q_OBJECT
public: 
    PayModes(QPushButton *_toggle_cash,
                  BarCore *core,
                  QWidget *parent=0,
                  const QString& name = QString());

    ~PayModes();

public slots:
    void mastercardClickedSlot(); //payByMastercard()
    void visaClickedSlot(); //payByVisa()
    void amexClickedSlot(); //payByAmex()
    void fourbClickedSlot();
    void cashClickedSlot(); //payByCash()
    void gourmetClickedSlot();
    void restaurantClickedSlot();
    void sodexhoClickedSlot();
    void proceccionCivilClickedSlot();

    void cancelClickedSlot(); //cancelPayment()

signals:
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);

protected:
    void setNewPrice(double price);
    void sendTicketSignal(const QString& pay_mode);

    void showEvent(QShowEvent *e);

    BarCore *core;
    QPushButton *toggle_cash;
};

#endif
