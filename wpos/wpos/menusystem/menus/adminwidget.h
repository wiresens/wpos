/***************************************************************************
                          bsladmonpanelwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLADMONPANELWIDGET_H
#define BSLADMONPANELWIDGET_H

#include "ui_adminwidget.h"

class XmlConfig;
class GenericButtonGroup;
class ProductScreenStack;
class QPushButton;
class QProcess;

class AdminWidget : public QWidget, private Ui::AdminWidget  {
    Q_OBJECT
public:
    explicit AdminWidget(ProductScreenStack *stack, QWidget* parent=nullptr, const QString& name = QString());
    ~AdminWidget();

    QPushButton *toggle_cash;
    QPushButton *toggle_derrama;
    QPushButton *toggle_anulation;
    QPushButton *toggle_out_screen;
    QPushButton *toggle_waiters;
    QPushButton *toggle_printing;
    QPushButton *toggle_fx2000;
    QPushButton *toggle_double_printing;

public slots:
    void cancelSlot();
    void launchBackOffice();
    void backOfficeEndSlot();
    int launchXterm();

    void loadToggleButtonStatus();
    void saveToggleButtonStatus();

    void genericSignalSlot(const QString& signal);

signals:
    void genericSignal(const QString& signal);
    void genericDataSignal(const QString& signal ,XmlConfig *xml);

protected:
    void init(ProductScreenStack *stack);
    void raiseMainWidget();
    void raiseConfigWidget();

    GenericButtonGroup *management;
    GenericButtonGroup *operations;
    GenericButtonGroup *cancel_operations;
    GenericButtonGroup *printer_operations;
    GenericButtonGroup *controls;
    GenericButtonGroup *admin_operations;

    QProcess *process;
};

#endif
