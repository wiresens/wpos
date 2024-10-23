/***************************************************************************
                          receiptrecoverwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "dbusreceiptprimitive_interface.h"
#include "dbusreceiptmediator_interface.h"

#include "receiptrecoverwidget.h"
#include "receiptrecoverwidget_adaptor.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/productextrainfo.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <wposgui/order/orderview.h>
#include <wposgui/order/ordercontentview.h>

#include "database/killticketsdb.h"
#include "customize/npopupkeyboard.h"
#include "salesscreen.h"

#include "auth/authcore.h"
#include "barcore/dbusreceiptquerier.h"

#include <QMenu>
#include <QStringList>
#include <QFile>
#include <QPushButton>
#include <QLayout>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QList>

extern AuthCore *authCore;

static const uint ITEMS_PER_PAGE {8};
static const uint TIME_OUT       {25};

using IReceiptPrimitive = com::wiresens::wpos::dbusreceipt::DBusReceiptPrimitive;
using IReceiptMediator  = com::wiresens::wpos::dbusreceipt::DBusReceiptMediator;

static const QString dbusService            = "com.wiresens.wpos.dbusreceipt";
static const QString dbusPrimitiveObject    = "/wpos/dbusreceipt/DBusReceiptPrimitive";
static const QString dbusMediatorObject     = "/wpos/dbusreceipt/DBusReceiptMediator";

const QString ReceiptRecoverWidget::DBusObject  = QString{"/wpos/wpos/ReceiptRecover"};

ReceiptRecoverWidget::ReceiptRecoverWidget(
    QWidget *parent,
    const QString& name)
    :TicketsHandlingWidget(parent, name)
{
    new ReceiptRecoverAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ReceiptRecoverWidget::DBusObject, this);

    invoice_button->hide();
    caption_label->setText(tr("Retrieve Commands"));
    m_receiptQuerier = new DBusReceiptQuerier(this);
    if ( QFile( cfg::xmlFileByKey(cfg::XMLKey::Lounges)).exists() )  m_use_lounges = true;

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_CHANGE_XML, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);

    gsm->publishGenericSignal(GSIGNAL::BARCORE_SET_LAST_RECEIPT, this);

    gsm->subscribeToGenericDataSignal(GDATASIGNAL::RECEIPTWIDGET_ALL_SETENABLED, this);

    m_lounge_button->setText(tr("Open\nCommand"));
    m_lounge_button->setIcon(m_null_pixmap);
    m_lounge_button->setIconSize(m_null_pixmap.size());
    m_lounge_button->setFixedSize(160, 60);

    m_pay_button->setIcon(m_pay_pixmap);
    allusers_button->setIcon(m_allusers_pixmap);
    m_pay_button->setIconSize(m_pay_button->size());
    allusers_button->setIconSize(m_allusers_pixmap.size());

    ticketnum_treeview->setSortingEnabled(true);
    ticketnum_treeview->setSelectionMode(QAbstractItemView::SingleSelection);
    ticketnum_treeview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ticketnum_treeview->setColumnWidth(OrderSection::Icon,          40);
    ticketnum_treeview->setColumnWidth(OrderSection::EmployeeName,  80);
    ticketnum_treeview->setColumnWidth(OrderSection::CommandId,     90);
    ticketnum_treeview->setColumnWidth(OrderSection::Date,          130);
    ticketnum_treeview->setColumnWidth(OrderSection::Status,        45);

    ticketnum_treeview->setHeaderLabels(
        QStringList()
        << QString("")
        << tr("Waiter")
        << tr("Ticket N°")
        << tr("Waiter's Id")
        << tr("Date")
        << tr("Status")
        << tr("Table N°")
    );

    ticketnum_treeview->hideColumn(OrderSection::EmployeeId);
//    ticketnum_treeview->hideColumn(OrderSection::Date);
    ticketnum_treeview->hideColumn(OrderSection::TableCode);

    connect(allusers_button, &QPushButton::toggled,
        this, &ReceiptRecoverWidget::usersButtonSlot);

    connect(m_pay_button, &QPushButton::clicked,this ,
        &ReceiptRecoverWidget::getReceiptSlot);

    connect(m_lounge_button, &QPushButton::clicked,
        this, &ReceiptRecoverWidget::newOrderClickedSlot);

    connect(ticketnum_treeview, &QTreeWidget::itemClicked,
        this, &ReceiptRecoverWidget::clickedSlot);
}

ReceiptRecoverWidget::~ReceiptRecoverWidget(){
    ticketnum_treeview->clear();
    delete m_receiptQuerier;
}

void ReceiptRecoverWidget::showEvent(QShowEvent *event){
    if ( ! allusers_button->isDown() )
        allusers_button->toggle();

    if ( ! refreshAndShow() ) return;

    XmlConfig xml;
    m_order_view->updateOrder(&xml);
    QFrame::showEvent(event);
}

void ReceiptRecoverWidget::hideEvent(QHideEvent *event){
    ticketnum_treeview->clear();
    if (m_npopup_keyboard){
        disconnect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
            this, &ReceiptRecoverWidget::kbExitClickedSlot);

        disconnect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
            this, &ReceiptRecoverWidget::kbTextChanged);

        disconnect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
            this, &ReceiptRecoverWidget::setUseLounge);

        m_npopup_keyboard->deleteLater();
        m_npopup_keyboard=0;

        ticketnum_treeview->setEnabled(true);
    }
    QFrame::hideEvent(event);
}

void ReceiptRecoverWidget::receiptChanged(
    QString employee_id,
    QString start_time)
{
    qDebug() << "DBus Service Request :  ReceiptRecoverWidget::receiptChanged";
    qDebug() <<"Employee Id : " << employee_id;
    qDebug() <<"Start Time : " << start_time;

    refreshAndShow();
    if (m_npopup_keyboard)  m_npopup_keyboard->setUsedTables(getUsedTableCodes());
}

bool ReceiptRecoverWidget::refreshAndShow(){

    m_pay_button->setEnabled(true);

    auto bus = QDBusConnection::sessionBus();
    auto remote_iface = new IReceiptPrimitive(dbusService, dbusPrimitiveObject, bus, this);
    auto local_iface = new IReceiptMediator(dbusService, dbusMediatorObject, bus, this);

    //check if the dbusreceipt is active
    if ( !remote_iface->isValid() || !local_iface->isValid()){
        qDebug() << bus.lastError().message();
        return false;
    }

    refreshList();
    return true;
}

void ReceiptRecoverWidget::refreshList(){

    //Here we talk to D-Bus to retrive the receipt resume
    ticketnum_treeview->clear();
    TicketResumes tickets;

    if ( ! allusers_button->isDown() )
        tickets =  employeeReceiptResume();
    else
        tickets = employeeReceiptResume( authCore->userId() );

    if ( tickets.isEmpty() ) return;

    for (const auto& ticket : tickets){
        auto item = new QTreeWidgetItem(ticketnum_treeview);

        if ( ticket.receipt_state ) item->setIcon( OrderSection::Icon, m_canceled_pixmap );
        else item->setIcon( OrderSection::Icon, m_null_pixmap );

        item->setText(OrderSection::EmployeeName, ticket.employee_name);

//        if ( ticket.table_code.isEmpty()) item->setText( OrderSection::CommandId, ticket.timestamp);
//        else item->setText( OrderSection::CommandId, ticket.table_code);

        if ( ticket.table_code.isEmpty()) item->setText( OrderSection::CommandId, "Counter");
        else item->setText( OrderSection::CommandId, ticket.table_code);

        item->setText(OrderSection::EmployeeId, ticket.employee_id);

        if ( ticket.receipt_state ) item->setText(OrderSection::Status, "locked");
        else item->setText(OrderSection::Status, "unlocked");

//        item->setText(OrderSection::Date, dateTimeString(ticket.timestamp, false));
        item->setText(OrderSection::Date, ticket.timestamp);
        item->setText(OrderSection::TableCode, ticket.table_code);
    }

    ticketnum_treeview->sortByColumn(OrderSection::TableCode, Qt::AscendingOrder);
}

void ReceiptRecoverWidget::clickedSlot(
    QTreeWidgetItem* item,
    int column)
{
    if (!item) return;
    if ( column == OrderSection::Icon &&
        item->text( OrderSection::Status ) == "unlocked" )
    {
        ticketnum_treeview->setEnabled(false);
        m_npopup_keyboard = new NPopUpKeyboard(this);
        m_npopup_keyboard->installEventFilter(this);

        connect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
            this, &ReceiptRecoverWidget::kbExitClickedSlot);

        connect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
            this, &ReceiptRecoverWidget::kbTextChanged);

        connect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
            this, &ReceiptRecoverWidget::setUseLounge);

        showMenu();
        handleTicketSelected();
        m_npopup_keyboard->setUsedTables( getUsedTableCodes());
        m_npopup_keyboard->setText( item->text( OrderSection::TableCode ) );

        if (m_use_lounges) m_npopup_keyboard->showLounge();
        else m_npopup_keyboard->showKeyBoard();
    }
}

void ReceiptRecoverWidget::kbExitClickedSlot(){
    int count;
    QString employee_id, aux;
    QString start_time, lounge, table;
    QTreeWidgetItem *item{};
    HList<ProductExtraInfo> *options{};

    ticketnum_treeview->setEnabled(false);
    m_input_text = m_input_text.simplified();

    if (!m_input_text.isEmpty()){
        item = ticketnum_treeview->currentItem();
        employee_id = item->text(OrderSection::EmployeeId);
        start_time  = item->text(OrderSection::Date);

        if ( m_receiptQuerier->receiptState(employee_id, start_time) )
            return;

        m_receiptQuerier->lockRemoteReceipts(employee_id, start_time);
        auto xml = m_receiptQuerier->getReceipt(employee_id, start_time);

        aux = m_input_text;
        xml.doWrite("description", m_input_text);
        if (m_npopup_keyboard->isAtTable()){
            //write lounges
            lounge = m_npopup_keyboard->getLounge();
            xml.doWrite("location.lounge", lounge);
            table = QString::number(m_npopup_keyboard->getTable());
            xml.doWrite("location.table", table);
            //write options
            options = m_npopup_keyboard->getOptionsFromLounge(lounge);
            if (options){
                xml.setDomain("products");
                xml.deleteElement("defaultoptions");
                xml.createElementSetDomain("defaultoptions");
                for(auto* node : *options){
                    xml.createElementSetDomain("option");
                    xml.createElement("type", node->getOptionType());
                    xml.createElement("value", node->getDefaultOption());
                    xml.releaseDomain("option");
                }
                xml.releaseDomain("defaultoptions");
            }
        }
        xml.delDomain();
        m_receiptQuerier->saveReceipt(&xml);
        m_receiptQuerier->unlockRemoteReceipts(employee_id, start_time);
        refreshList();
    }
    disconnect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
        this, &ReceiptRecoverWidget::kbExitClickedSlot);

    disconnect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
        this, &ReceiptRecoverWidget::kbTextChanged);

    disconnect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
        this, &ReceiptRecoverWidget::setUseLounge);

    m_npopup_keyboard->deleteLater();
    m_npopup_keyboard=0;

    if (!aux.isEmpty())
        QTimer::singleShot(TIME_OUT, this,
        &ReceiptRecoverWidget::setSelectedSlot);
    ticketnum_treeview->setEnabled(true);

}

void ReceiptRecoverWidget::setSelectedSlot(){
    QTreeWidgetItem *item = 0;
    if (!m_input_text.isEmpty()){
        item = ticketnum_treeview->findItems(m_input_text, Qt::MatchExactly, OrderSection::TableCode).first();
        if (item){
            ticketnum_treeview->setCurrentItem(item);
            item->setHidden(false);
            item->setSelected(true);
        }
        m_input_text.clear();
    }
}

void ReceiptRecoverWidget::kbTextChanged(const QString& text){
    m_input_text = text;
}

void ReceiptRecoverWidget::handleTicketSelected() {

    auto selected_items = ticketnum_treeview->selectedItems();
    if (selected_items.isEmpty()) return;

    /* Get the ticket ID. */
    // We take the first item in the selection
    //since we are in single row selection
    auto item = selected_items.at(0);

    auto employee_id    = item->text(OrderSection::EmployeeId);
    auto start_time     = item->text(OrderSection::Date);
    auto status         = item->text(OrderSection::Status);

    m_pay_button->setEnabled(  ! (status == "locked") );

    //get the xml from the receipt
    XmlConfig xml { m_receiptQuerier->getReceipt(employee_id, start_time)};

    m_order_view->updateOrder(&xml);
    m_order_view->contentView()->selectFirst();
}

void ReceiptRecoverWidget::getReceiptSlot(){

    QTreeWidgetItem* item =  ticketnum_treeview->currentItem();
    if (!item) return;

    auto employee_id = item->text(OrderSection::EmployeeId);
    auto start_time = item->text(OrderSection::Date);

    //get the xml from the receipt
    XmlConfig xml {m_receiptQuerier->getReceipt(employee_id, start_time)};
    emit genericDataSignal (GDATASIGNAL::BARCORE_CHANGE_XML, &xml);
    m_receiptQuerier->lockRemoteReceipts(employee_id, start_time);

    XmlConfig tmp_xml;
    tmp_xml.createElement("name", SalesScreen::PAY_MODE_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &tmp_xml);

    tmp_xml.deleteElement("name");
    tmp_xml.createElement("enabled", "false");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &tmp_xml);
}

void ReceiptRecoverWidget::rejectChange(){
    XmlConfig xml;

    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);

    xml.deleteElement("name");
    xml.createElement("enabled","true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
    emit genericSignal(GSIGNAL::BARCORE_SET_LAST_RECEIPT);
}

void ReceiptRecoverWidget::usersButtonSlot(bool on){
    (void) on;
    refreshAndShow();
}

void ReceiptRecoverWidget::genericDataSignalSlot(
    const QString& signal_name,
    XmlConfig *xml)
{
    if (signal_name == GDATASIGNAL::RECEIPTWIDGET_ALL_SETENABLED){
        QString aux;
        xml->delDomain();
        aux = xml->readString("enabled");
        aux = aux.toLower();
        if (aux == "true"){
            allusers_button->show();
            allusers_button->setDown(false);
        }
        else if (aux == "false"){
            allusers_button->setDown(false);
            allusers_button->hide();
        }
    }
}

void ReceiptRecoverWidget::setUseLounge(bool enable){
    m_use_lounges = enable;
}

QStringList ReceiptRecoverWidget::getUsedTableCodes(){
    m_ticket_db->connect();
        TicketResumes tickets = m_ticket_db->getReceiptResume();
    m_ticket_db->disConnect();

    QStringList table_codes;
    for ( TicketResumeData& ticket : tickets)
        if (!ticket.table_code.isEmpty())
            table_codes.append(ticket.table_code);

    return table_codes;
}

void ReceiptRecoverWidget::newOrderClickedSlot()
{
    ticketnum_treeview->setEnabled(false);
    m_npopup_keyboard = new NPopUpKeyboard(this);
    m_npopup_keyboard->installEventFilter(this);

    connect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
        this, &ReceiptRecoverWidget::openOrderKbExitClickedSlot);

    connect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
        this, &ReceiptRecoverWidget::kbTextChanged);

    connect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
        this, &ReceiptRecoverWidget::setUseLounge);

    showMenu();
    m_npopup_keyboard->setUsedTables(getUsedTableCodes());

    if (m_use_lounges) m_npopup_keyboard->showLounge();
    else m_npopup_keyboard->showKeyBoard();
}

void ReceiptRecoverWidget::openOrderKbExitClickedSlot(){
    XmlConfig *xml{};
    m_input_text = m_input_text.simplified();
    if (!m_input_text.isEmpty()){
        XmlConfig xml;
        xml.delDomain();
        xml.doWrite("description", m_input_text);
        auto name = authCore->userName()+" "+authCore->userLastName();
        xml.createElementSetDomain("employee");
        xml.createElement("name",name);
        xml.createElement("dni", authCore->userId());
        xml.delDomain();

        auto start_date = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
        xml.createElement("timestamps.start", start_date);
        auto last_date = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);;
        xml.createElement("timestamps.last", last_date);

        xml.createElement("location.lounge", "venta directa");
        xml.createElement("location.table", "0");
        if (m_npopup_keyboard->isAtTable()){
            //write lounges
            auto lounge = m_npopup_keyboard->getLounge();
            xml.doWrite("location.lounge",lounge);
            auto table = QString::number(m_npopup_keyboard->getTable());
            xml.doWrite("location.table", table);
            //write options
            auto options = m_npopup_keyboard->getOptionsFromLounge(lounge);
            if (options){
                if (!xml.setDomain("products"))
                    xml.createElementSetDomain("products");

                xml.deleteElement("defaultoptions");
                xml.createElementSetDomain("defaultoptions");

                for(auto node : *options){
                    xml.createElementSetDomain("option");
                    xml.createElement("type", node->getOptionType());
                    xml.createElement("value", node->getDefaultOption());
                    xml.releaseDomain("option");
                }
                xml.releaseDomain("defaultoptions");
            }
        }
        xml.delDomain();
        m_receiptQuerier->createReceipt(&xml);
        refreshList();
    }

    disconnect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
        this, &ReceiptRecoverWidget::openOrderKbExitClickedSlot);

    disconnect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
        this, &ReceiptRecoverWidget::kbTextChanged);

    disconnect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
        this, &ReceiptRecoverWidget::setUseLounge);

    m_npopup_keyboard->deleteLater();
    m_npopup_keyboard=0;

    if ( !m_input_text.isEmpty() )
        QTimer::singleShot( TIME_OUT, this,
            &ReceiptRecoverWidget::setSelectedSlot);

    ticketnum_treeview->setEnabled(true);
}

bool ReceiptRecoverWidget::eventFilter(
        QObject *watched,
        QEvent *event)
{
    auto filtered = event->type() == QEvent::Hide &&
                    watched == m_npopup_keyboard;

    if ( filtered ){
        m_npopup_keyboard->close();
        disconnect(m_npopup_keyboard, &NPopUpKeyboard::exitClicked,
            this, &ReceiptRecoverWidget::openOrderKbExitClickedSlot);

        disconnect(m_npopup_keyboard, &NPopUpKeyboard::textChanged,
            this, &ReceiptRecoverWidget::kbTextChanged);

        disconnect(m_npopup_keyboard, &NPopUpKeyboard::loungeRequested,
            this, &ReceiptRecoverWidget::setUseLounge);

        m_npopup_keyboard->deleteLater();
        m_npopup_keyboard=0;
        ticketnum_treeview->setEnabled(true);
    }
    TicketsHandlingWidget::eventFilter(watched, event);
    return filtered;
}

ReceiptRecoverWidget::TicketResumes
ReceiptRecoverWidget::employeeReceiptResume(
    const QString& employee_id)
{
    return receiptResumeFromXml( m_receiptQuerier->getReceiptResume(employee_id) );
}

ReceiptRecoverWidget::TicketResumes
ReceiptRecoverWidget::employeeReceiptResume(){
    return receiptResumeFromXml(m_receiptQuerier->getReceiptResume());
}

ReceiptRecoverWidget::TicketResumes
ReceiptRecoverWidget::receiptResumeFromXml(XmlConfig xml)
{
    QVector<TicketResumeData> ticket_resumes;
    xml.delDomain();
    xml.setDomain("receipts");

    for (auto i=0; i < xml.howManyTags("receipt"); i++){
        xml.setDomain("receipt["+QString::number(i)+"]");
        TicketResumeData ticket_resume;
        ticket_resume.employee_id = xml.readString("employee_id");
        ticket_resume.timestamp = xml.readString("start_time");
        auto aux = xml.readString("blocked");

        if ( aux=="true" || aux=="TRUE" )
            ticket_resume.receipt_state = true;
        else
            ticket_resume.receipt_state = false;

        ticket_resume.employee_name = xml.readString("employee_name");
        ticket_resume.table_code = xml.readString("description");
        xml.releaseDomain("receipt");
        ticket_resumes.push_back(ticket_resume);
    }
    return ticket_resumes;
}

void ReceiptRecoverWidget::showMenu(){
    auto low  = m_lounge_button->mapToGlobal( QPoint(0,0));
    auto hight  = ticketnum_treeview->mapToGlobal( QPoint(0,0));

    auto pos = ticketnum_treeview->mapToGlobal(  QPoint( 0, (low.y() + hight.y()) / 2));
    m_npopup_keyboard->showMenu( pos );
}
