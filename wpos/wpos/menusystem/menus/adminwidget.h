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
    explicit AdminWidget(
        ProductScreenStack *stack,
        QWidget* parent=nullptr,
        const QString& name = QString{});
signals:
    void genericSignal(const QString& signal);
    void genericDataSignal(const QString& signal ,XmlConfig *xml);

private slots:
    void cancelSlot();
    void launchBackOffice();
    void restoreState();
    int launchXterm();

    void loadToggleButtonStatus();
    void saveToggleButtonStatus();

    void genericSignalSlot(const QString& signal);

private:
    void init(ProductScreenStack *stack);
    void raiseMainWidget();
    void raiseConfigWidget();

public:
    QPushButton *toggle_cash{};
    QPushButton *toggle_derrama{};
    QPushButton *toggle_anulation{};
    QPushButton *toggle_out_screen{};
    QPushButton *toggle_waiters{};
    QPushButton *toggle_printing{};
    QPushButton *toggle_fx2000{};
    QPushButton *toggle_double_printing{};

private:
    GenericButtonGroup *management{};
    GenericButtonGroup *operations{};
    GenericButtonGroup *cancel_operations{};
    GenericButtonGroup *printer_operations{};
    GenericButtonGroup *controls{};
    GenericButtonGroup *admin_operations{};

    QProcess *process{};
    const QString wposbo;
};

#endif
