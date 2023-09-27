/***************************************************************************
                          pantallapresentacion.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef SALES_SCREEN_H
#define SALES_SCREEN_H

#include <QtWidgets/QtWidgets>
#include "ui_saleswidget.h"

class XmlConfig;
class ProductScreenStack;
class NumKeyboardBox;
class OrderPickerView;
class BarCore;
class TicketCore;
class QShowEvent;
class QHBoxLayout;
class MenuStack;
class ButtonSetWidget;
class EventLogCore;
class AdminWidget;
class GenericButtonGroup;

class SalesScreen : public QWidget,  private Ui::SalesWidget{
    Q_OBJECT
public:
    static const QString PRODUCT_MENU;
    static const QString AUTH_MENU;
    static const QString FREE_PRICE_MENU;
    static const QString CASH_MENU;
    static const QString XZ_MENU;
    static const QString CHANGE_MONEY_MENU;
    static const QString EXIT_MENU;
    static const QString KILL_TICKETS_MENU;
    static const QString RECEIPT_MENU;
    static const QString INVITATION_MENU;
    static const QString ADMIN_MENU;
    static const QString GENERIC_PRODUCT_MENU;
    static const QString PAY_MODE_MENU;

    SalesScreen(QWidget *parent, QSplashScreen& splash_screen, const QString& name = QString(""));

public slots:
    void receiveProduct(XmlConfig *xml);
    void receiveCash();
    void setDefaultState();

    void genericSignalSlot(const QString& signal_name);
    void genericDataSignalSlot(const QString& signal_name, XmlConfig *xml);

signals:
    void setMenu(int);
    void setScreen(const QString &);
    void genericSignal(const QString& signal_name);
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
    void splashRequested(const QString &, int, const QColor &);

protected slots:
    void checkPendingEvents();

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void setEnabledLateralWidgets(bool enabled);
    void setVisibleLateralWidgets(bool visible);

    void createAuthenticationDialog();
    void createFreepriceDialog();
    void createBalanceCollectionDialog();
    void createCashMovementDialog();
    void createGenericProductDialog();
    void createExitActionDialog();    
    void createInvitationDialog();

    void createReportDialog();
    void createTicketCancellationDialog();
    void createReceiptRecoverDialog();
    void createAdminDialog();

    void createProductDialog();
    void createShortcutButton();
    void createPayByCashButton();
    void createOpenCashDeviceButton();
    void createPaySelectionDialog();

    OrderPickerView *orderManager{};
    MenuStack *menu_stack{};
    ProductScreenStack *product_screen_stack{};
    AdminWidget *admin_panel{};
    GenericButtonGroup *shortcut_group{};
    GenericButtonGroup *openbox_and_paymode_buttons_group{};
    NumKeyboardBox *numkey{};
    ButtonSetWidget *upper_buttons{};

    BarCore *bar_core{};
    TicketCore *ticket_core{};
    EventLogCore *event_log_core;

    QHBoxLayout *shortcut_layout{};
    QFrame *sp_frame{};

    QTimer *timer{};
    uint counter{};
};

#endif
