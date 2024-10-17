#include "ticketshandlingwidget.h"

#include "database/killticketsdb.h"

#include <wposgui/order/orderview.h>
#include <wposgui/order/ordercontentview.h>
#include <wposgui/common/toolkit.h>
#include <wposcore/config.h>

TicketsHandlingWidget::TicketsHandlingWidget(
        QWidget *parent, const QString& name):

    QFrame(parent),
    m_ticket_db { new KillTicketsDB(name, cfg::xmlFileByKey(cfg::XMLKey::Database)) }

{
    setupUi(this);
    setObjectName(name);
/** There is a bug somewhere. When this is enabled here and suppress in the subclasses,
 *  Qt is able to connect the signal for GDATASIGNAL_MAINSTACK_SETPAGE but not that of
 *  GDATASIGNAL_BARCORE_CHANGE_XML. It sends the error message:
 *  QObject::connect: No such signal QFrame::genericDataSignal(const QString&, XmlConfig *) in genericsignalmanager.cpp:122
 *  QObject::connect:  (sender name:   'KillTicketsWidget')
 *  QObject::connect:  (receiver name: 'bar_core')
 *  This means it's looking for the signal in a QFrame class, where it cannot actually find.
 *  Why is it that for only one of the signals but not both ?
 */
//    if (generic_signal_manager){
//        generic_signal_manager->publishGenericDataSignal(GDATASIGNAL_MAINSTACK_SETPAGE, this);
//        generic_signal_manager->publishGenericDataSignal(GDATASIGNAL_BARCORE_CHANGE_XML, this);
//    }

    m_order_view = new OrderView(order_frame, "ORDER");

    allusers_button->hide();
    ticket_down_button->setAutoRepeat(true);
    ticket_up_button->setAutoRepeat(true);
    order_down_button->setAutoRepeat(true);
    order_up_button->setAutoRepeat(true);

    ticket_down_button->setIcon(QPixmap("controls:down_48.png"));
    ticket_up_button->setIcon(QPixmap("controls:up_48.png"));
    order_down_button->setIcon(QPixmap("controls:down_48.png"));
    order_up_button->setIcon(QPixmap("controls:up_48.png"));
    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));


    auto order_layout = new QVBoxLayout(order_frame);
    order_layout->addWidget(m_order_view);
    order_layout->setContentsMargins(0,0,0,0);

    connect(ticket_down_button, &QAbstractButton::clicked, this, &TicketsHandlingWidget::selectDown);
    connect(ticket_up_button, &QAbstractButton::clicked, this, &TicketsHandlingWidget::selectUp);
    connect(order_down_button, &QAbstractButton::clicked, m_order_view->contentView(), &OrderContentView::selectDown);
    connect(order_up_button, &QAbstractButton::clicked, m_order_view->contentView(), &OrderContentView::selectUp);
    connect(cancel_button, &QAbstractButton::clicked, this, &TicketsHandlingWidget::rejectChange);
    connect(ticketnum_treeview, &QTreeWidget::itemSelectionChanged, this, &TicketsHandlingWidget::handleTicketSelected);
}

TicketsHandlingWidget::~TicketsHandlingWidget(){
    delete m_order_view;
    delete m_ticket_db;
}

void TicketsHandlingWidget::selectDown(){
    selectTreeItemDown(ticketnum_treeview);
}

void TicketsHandlingWidget::selectUp(){
    selectTreeItemUp(ticketnum_treeview);
}
