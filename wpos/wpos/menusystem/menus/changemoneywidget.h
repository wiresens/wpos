/***************************************************************************
                          bslchangemoneywidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef CHANGEMONEYWIDGET_H
#define CHANGEMONEYWIDGET_H

#include "ui_changemoneywidget.h"
#include "database/changemoneywidgetdb.h"
#include <QWidget>

class FloatKeyboard;
class XmlConfig;

class ChangeMoneyWidget : public QWidget, private Ui::ChangeMoneyWidget  {
Q_OBJECT
  public:
    explicit ChangeMoneyWidget(QWidget *parent=0, const QString& name = QString{});
    void showEvent(QShowEvent *event);

  signals:
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);

  public slots:
    void updateChangeSlot(double change);
    void acceptChangeSlot();
    void rejectChangeSlot();
    void sustractMoneySlot();
    void addMoneySlot();
    void toggleButtonsStateSlot();

  private:
    FloatKeyboard *keyPad;
    ChangeMoneyWidgetDB *db;

    /* This variable stores how many money is in the cash BEFORE
     * the change is done.
     */
    double money_in_cash;

    /* This variable stores the value of the variation (always positive) */
    double variation;

    /* This variable uses the OperationType enum, it stores if we are
     * adding or substracting.
     */
    int operation_type;

    enum {
        ADD_UP,
        SUBSTRACT
    } OperationType;
};

#endif
