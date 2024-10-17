// file      :  salesscreen.h
// birth     :  06/19/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef SALES_SCREEN_H
#define SALES_SCREEN_H

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

namespace Ui{
class SalesWidget;
}

class SalesScreen :  public QWidget
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
        const QString& name = QString{}
    );

    ~SalesScreen(){ delete ui;}

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
    void showGreeter();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void enableLateralWidgets(bool enabled);
    void showLateralWidgets(bool visible);

    ProductScreenStack *createProductDialog(MenuStack *screens);
    void createAuthenticationDialog(MenuStack *screens);

    void createFreepriceDialog(MenuStack *screens);
    void createBalanceCollectionDialog(MenuStack *screens);
    void createCashMovementDialog(MenuStack *screens);
    void createExitActionDialog(MenuStack *screens);
    void createReportDialog(MenuStack *screens);

    void createTicketCancellationDialog(MenuStack *screens);
    void createReceiptRecoverDialog(MenuStack *screens);

    void createInvitationDialog(MenuStack *screens);
    void createPayByCashButton();
    void createGenericProductDialog(MenuStack *screens);

    void createAdminDialog(MenuStack *screens);
    void createPaySelectionDialog(MenuStack *screens);

    void createShortcutButton();
    void createOpenCashDeviceButton();

private:
    Ui::SalesWidget     *ui{};
    MenuStack           *m_screens{};      // StackedWidget containing all screens
    OrderPickerView     *m_order_picker_view{};
    ProductScreenStack  *m_product_screens{};
    AdminWidget         *m_admin_wgt{};
    GenericButtonGroup  *m_shortcut_grp{};
    GenericButtonGroup  *m_paymode_grp{};
    NumKeyboard         *m_num_keyboard{};
    ButtonSetWidget     *m_upper_btns{};

    BarCore             *m_bar_core{};
    TicketCore          *m_ticket_core{};
    EventLogCore        *m_eventlog_core;

    QHBoxLayout         *m_shortcut_layout{};

};

#endif
