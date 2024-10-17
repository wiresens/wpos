/***************************************************************************
                          bslbuttonsetwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BUTTONSETWIDGET_H
#define BUTTONSETWIDGET_H

#include "ui_buttonsetwidget.h"
#include <QButtonGroup>

#include <wposcore/hlist.h>

class QShowEvent;
class ProductScreenStack;
class GenericButtonGroup;

class ButtonSetWidget : public QWidget,  private Ui::ButtonSetWidget{
    Q_OBJECT
public:
    explicit ButtonSetWidget(
        ProductScreenStack* stack,
        QWidget* parent=nullptr,
        const QString& name = QString{});
    ~ButtonSetWidget();

public slots:
    void genericSignalSlot(const QString& signal_name);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void createButtons();

private:
    ProductScreenStack *m_product_screen_stack{};
    GenericButtonGroup *m_generic_btn_grp{};
};

#endif
