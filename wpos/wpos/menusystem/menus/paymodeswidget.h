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

class PayModes :
    public QWidget,
    private Ui::PayModesWidget {

    Q_OBJECT
public:
    PayModes(
        QPushButton *_toggle_cash,
        BarCore *core,
        QWidget *parent=0,
        const QString& name = QString{});

    ~PayModes() = default;

public slots:
    void mastercardPay(); //payByMastercard()
    void visaPay(); //payByVisa()
    void amexPay(); //payByAmex()
    void fourbPay();
    void cashPay(); //payByCash()
    void gourmetPay();
    void restaurantPay();
    void sodexhoPay();
    void proceccionPay();

    void cancelPayment(); //cancelPayment()

signals:
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);

protected:
    void showEvent(QShowEvent *e);

private:
    void setNewPrice(double price);
    void sendTicketSignal(const QString& pay_mode);
    void connectButtons();

private:
    BarCore *core;
    QPushButton *toggle_cash;
};

#endif
