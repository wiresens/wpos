/***************************************************************************
                          bslcashwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef CASHWIDGET_H
#define CASHWIDGET_H

#include "ui_cashwidget.h"
#include <QWidget>


class FloatKeyboardBox;
class BarCore;
class XmlConfig;

class CashWidget : public QWidget, private Ui::CashWidget  {
    Q_OBJECT
public:
    CashWidget(BarCore *_core,
                  QWidget *parent, 
                  const QString& name = QString());
    ~CashWidget();

    void setNewPrice(double price);

protected slots:
    void numkeyChangedSlot(double cash);

    void accepSlot();
    void cancelSlot();

    void quickAccept();
    void genericDataSignalSlot(const QString& signal_name,XmlConfig *xml);

signals:
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);
    void genericSignal(const QString& signal_name);

protected:
    void showEvent(QShowEvent *e);
    void sendTicketSignal();

    FloatKeyboardBox *float_keyboard;
    BarCore *core;
    double cash;
    double actual_price;
    bool fast_cashing{ true }; // Controls whether clicking the ok_button continue the sales operation
    bool cashing_completed;
};

#endif
