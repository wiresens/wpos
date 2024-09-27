// file      :  salesscreen.h
// birth     :  06/19/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef SALES_SCREEN_H
#define SALES_SCREEN_H

#include "ui_saleswidget.h"
#include <QtWidgets/QtWidgets>

class XmlConfig;
class ProductScreenStack;
class NumKeyboard;
class OrderPickerView;
class BarCore;
class TicketCore;
class QShowEvent;
class QHBoxLayout;
class MenuStack;
class MenuPage;
class ButtonSetWidget;
class EventLogCore;
class AdminWidget;
class GenericButtonGroup;

class SalesScreen :
    public QWidget,
    private Ui::SalesWidget
{
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

    SalesScreen(
        MenuPage *parent,
        QSplashScreen& splashScreen,
        const QString& name = QString()
    );

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
    void checkSuspend();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void setEnabledLateralWidgets(bool enabled);
    void setVisibleLateralWidgets(bool visible);

    void createProductDialog();
    void createAuthenticationDialog();

    void createFreepriceDialog();
    void createBalanceCollectionDialog();
    void createCashMovementDialog();
    void createExitActionDialog();
    void createReportDialog();

    void createTicketCancellationDialog();
    void createReceiptRecoverDialog();

    void createInvitationDialog();
    void createPayByCashButton();
    void createGenericProductDialog();

    void createAdminDialog();
    void createPaySelectionDialog();

    void createShortcutButton();
    void createOpenCashDeviceButton();

private:
    MenuStack           *menuStack{};      // StackedWidget containing all screens
    OrderPickerView     *orderPicker{};
    ProductScreenStack  *productScreenStack{};
    AdminWidget         *adminPanel{};
    GenericButtonGroup  *shortcutGroup{};
    GenericButtonGroup  *paymodeGroup{};
    NumKeyboard         *numkey{};
    ButtonSetWidget     *upperButtons{};

    BarCore             *barCore{};
    TicketCore          *ticketCore{};
    EventLogCore        *eventLogCore;

    QHBoxLayout         *shortcutLayout{};
    QFrame              *specialButtonsFrame{};

    QTimer              *timer{};
    uint                counter{};
};

#endif
