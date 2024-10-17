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

#include <QtWidgets/QWidget>

namespace Ui {
class CashWidget;
} // namespace Ui

class FloatKeyboard;
class BarCore;
class XmlConfig;

class CashWidget :
    public QWidget{
    Q_OBJECT

public:
    CashWidget(
        BarCore *barCore,
        QWidget *parent,
        const QString& name = QString{}
    );

    ~CashWidget();

    void setNewPrice(double price);

protected slots:
    void numkeyChangedSlot(double cash);

    void accepSlot();
    void cancelSlot();

    void quickAccept();
    void genericDataSignalSlot(const QString& signal,XmlConfig *xml);

signals:
    void genericDataSignal(const QString& signal,XmlConfig *xml);
    void genericSignal(const QString& signal);

protected:
    void showEvent(QShowEvent *event);
    void sendTicketSignal();

private:
    Ui::CashWidget *ui;

    FloatKeyboard *float_keyboard;
    BarCore *barCore{};
    double cash{};
    double actual_price{};
    bool fast_cashing{ true }; // Controls whether clicking the ok_button continue the sales operation
    bool cashing_completed{};
};

#endif
