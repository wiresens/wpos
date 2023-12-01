/***************************************************************************
                          bslkillticketswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "killticketswidget.h"

#include <wposcore/genericsignalmanager.h>
#include <xmlconfig.h>
#include <wposwidget/orderview.h>
#include <wposwidget/ordercontentview.h>

#include "database/killticketsdb.h"

#include "salesscreen.h"


#include "auth/authcore.h"
#include "printer/printermanager.h"

#include <QStringList>
#include <QListView>
#include <QFile>
#include <QPushButton>
#include <QLayout>
#include <QPixmap>
#include <QPainter>

#include <iostream>
using namespace std;

#define ITEMS_PER_PAGE 10
extern AuthCore *authCore;


KillTicketsWidget::KillTicketsWidget(
        QWidget *parent,
        const QString& name):

    TicketsHandlingWidget(parent, name)
{

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_CHANGE_XML, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::BARCORE_PROCESS_CORE, this);

    invoice_button->setIcon(invoice_pixmap);
    trash_button->setIcon(trash_pixmap);
    printer_button->setIcon(printer_pixmap);

    trash_button->setIconSize(trash_pixmap.size());
    //    printer_button->setIconSize(printer_pixmap.size());

    trash_button->setText(tr("Cancel\nTicket"));
    printer_button->setText(tr("Print\nTicket"));
    invoice_button->setText(tr("Print\nInvoice"));

    ticketnum_treeview->setSortingEnabled(true);
    ticketnum_treeview->setSelectionMode(QAbstractItemView::SingleSelection);
    ticketnum_treeview->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ticketnum_treeview->setColumnWidth(ResumeSection::Icon, 45);
    ticketnum_treeview->setColumnWidth(ResumeSection::TicketId, 45);
    ticketnum_treeview->setColumnWidth(ResumeSection::EmployeeName, 110);
    ticketnum_treeview->setColumnWidth(ResumeSection::Date, 110);
    ticketnum_treeview->setColumnWidth(ResumeSection::Total, 60);
    ticketnum_treeview->setColumnWidth(ResumeSection::Status, 60);

//    ticketnum_tree_view->headerItem()->setTextAlignment(ResumeSection::TicketId, Qt::AlignRight);
//    ticketnum_tree_view->headerItem()->setTextAlignment(ResumeSection::EmployeeName, Qt::AlignLeft);
//    ticketnum_tree_view->headerItem()->setTextAlignment(ResumeSection::Date, Qt::AlignHCenter);
//    ticketnum_tree_view->headerItem()->setTextAlignment(ResumeSection::Total, Qt::AlignRight);
//    ticketnum_tree_view->headerItem()->setTextAlignment(ResumeSection::Status, Qt::AlignLeft);



    ticketnum_treeview->setHeaderLabels(
                QStringList()
                << QString("")
                << tr("N°")
                << tr("Employee")
                << tr("Date")
                << tr("Total")
                << tr("Status"));
    for(auto i = 0 ; i < ticketnum_treeview->header()->count(); i++ )
        ticketnum_treeview->headerItem()->setTextAlignment(i, Qt::AlignHCenter);

    connect(invoice_button, &QAbstractButton::released, this, &KillTicketsWidget::printInvoice);
    connect(trash_button, &QAbstractButton::clicked, this, &KillTicketsWidget::deleteTicket);
    connect(printer_button, &QAbstractButton::released, this, &KillTicketsWidget::printTicket);
    connect(this, &KillTicketsWidget::genericDataSignal, order->orderContent(), &OrderContentView::genericDataSignalSlot);
}

KillTicketsWidget::~KillTicketsWidget(){
    ticketnum_treeview->clear();
}

void KillTicketsWidget::refreshList(){

    ticketnum_treeview->clear();
    ticket_db->connect();
    std::unique_ptr<QList<TicketResumeData*>> tickets { ticket_db->getTicketResume() };
    ticket_db->disConnect();

    if ( ! tickets.get() )  return;

    for ( auto& ticket : *tickets){
        auto item = new QTreeWidgetItem(ticketnum_treeview);
        if (ticket->ticket_state == "cancelado")
            item->setIcon(ResumeSection::Icon, anulation_pixmap);
        else if ( ticket->ticket_state == "cobrado" )
            item->setIcon(ResumeSection::Icon,  null_pixmap);
        else if ( ticket->ticket_state == "facturado" )
            item->setIcon(ResumeSection::Icon, invoice_pixmap);


        item->setText(ResumeSection::TicketId, ticket->ticket_code);
        item->setText(ResumeSection::EmployeeName, ticket->employee_name);
        item->setText(ResumeSection::Date, ticket->timestamp);
        item->setText(ResumeSection::Total, ticket->total);
        item->setText(ResumeSection::Status, ticket->ticket_state);

//        item->setTextAlignment(ResumeSection::TicketId, Qt::AlignRight);
//        item->setTextAlignment(ResumeSection::EmployeeName, Qt::AlignLeft);
//        item->setTextAlignment(ResumeSection::Date, Qt::AlignHCenter);
//        item->setTextAlignment(ResumeSection::Total, Qt::AlignRight);
//        item->setTextAlignment(ResumeSection::Status, Qt::AlignLeft);

        for(auto i = 0 ; i < ticketnum_treeview->header()->count(); i++ )
            item->setTextAlignment(i, Qt::AlignHCenter);
    }

    tickets->clear();
}

void KillTicketsWidget::showEvent(QShowEvent *event){

    refreshList();
    //@benes    ticketnum_listview->ensureItemVisible(ticketnum_listview->firstChild());
    if(ticketnum_treeview->topLevelItem(0))
        ticketnum_treeview->topLevelItem(0)->setHidden(false);

    XmlConfig xml;
    if (event)  order->changeData(&xml);
    QFrame::showEvent(event);
}

void KillTicketsWidget::hideEvent(QHideEvent *event){
    ticketnum_treeview->clear();
    QFrame::hideEvent(event);
}

void KillTicketsWidget::handleTicketSelected() {

    auto selected_items = ticketnum_treeview->selectedItems();
    if ( selected_items.isEmpty()) return;

    /* Get the ticket ID. */
    // We take the first item in the selection
    //since we are in single row selection
    auto selected_item = selected_items.at(0);
    auto ticket_id = selected_item->text(1).toInt();

    /* Check if the ticket has negative item */
    ticket_db->connect();

    bool ticket_is_cancelled = ticket_db->ticketHasNegative(QString::number(ticket_id));
    trash_button->setEnabled (!ticket_is_cancelled);

    auto  state = selected_item->text(5);

    invoice_button->setEnabled(false);
    if ( state == "cobrado" ){
        invoice_button->setEnabled(true);
        invoice_button->setText(tr("Print\nInvoice"));
    }
    else if ( state == "facturado"){
        invoice_button->setEnabled(true);
        invoice_button->setText(tr("Copy\nInvoice"));
    }

    XmlConfig xml ;
    if (ticket_is_cancelled)
        xml.createElement ("mode", "product_anulation");
    else
        xml.createElement ("mode", "normal");

    emit genericDataSignal (GDATASIGNAL::SETCORE_MODE, &xml);

    /* Get the ticket and Update the command */
    std::unique_ptr<XmlConfig> xml2  {ticket_db->getTicketFromDatabase(ticket_id)};
    order->changeData(xml2.get());
    order->orderContent()->selectFirst();
    ticket_db->disConnect();
}

void KillTicketsWidget::deleteTicket(){
    int tid = 0;
    QTreeWidgetItem *item = 0;

    item = ticketnum_treeview->currentItem();
    if (!item)
        return;

    if (!ticketnum_treeview->isItemSelected(item))
        return;

    tid = item->text (1).toInt ();

    ticket_db->connect();
    std::unique_ptr<XmlConfig> xml  {ticket_db->getTicketFromDatabase (tid)};
    ticket_db->disConnect();

    xml->doWrite ("employee.dni", authCore->userId());
    xml->doWrite ("employee.name", authCore->userName());
    setTicketNegative(xml.get());
    emit genericDataSignal (GDATASIGNAL::BARCORE_CHANGE_XML, xml.get());

    xml.reset( new XmlConfig());
    xml->createElement ("pay_type", "anulacion");
    emit genericDataSignal (GDATASIGNAL::BARCORE_PROCESS_CORE, xml.get());

    refreshList();
}

void KillTicketsWidget::rejectChange(){
    XmlConfig xml;
    xml.createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void KillTicketsWidget::setTicketNegative (XmlConfig *xml) {

    /* This function will return a negative ticket */
    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("products");

    for ( auto i = 0; i < xml->howManyTags ("product"); i++) {
        auto price = QString::number(xml->readString("product["+QString::number (i)+"].billing.price").toDouble() * -1);
        xml->doWrite ("product["+QString::number (i)+"].billing.price", price);
    }
    xml->popDomain ();
}

void KillTicketsWidget::printTicket(){

    /* Get the ticket ID */
    auto item = ticketnum_treeview->currentItem();
    if ( !item || !item->isSelected() ) return;

    auto tickect_id = item->text(1).toInt();

    /* Get the ticket */
    ticket_db->connect();
    std::unique_ptr<XmlConfig> xml { ticket_db->getTicketFromDatabase(tickect_id)};
    ticket_db->disConnect();

    /* Update the command */
    if (xml->wellFormed()){
        PrinterManager printer(nullptr, "killTicketPrinter");
        printer.printTicket( xml.get(), 1);
        xml->debug();
    }
}

void KillTicketsWidget::printInvoice(){
/**
    In handleTicketSelected() you have to check if the ticket already has an invoice
    to enable the invoice button to get a copy of the invoice.

    Check if the ticket is already billed. If so, get the invoice and ticket numbers.
    Generate the invoice xml and send to printer if not.

    Generate invoice number, change ticket status from charged to invoiced,
    associate ticket with invoice, generate invoice xml and send to printer
**/

    QTreeWidgetItem *item = ticketnum_treeview->currentItem();
    if (  !item || !ticketnum_treeview->isItemSelected(item) )  return;

    int tickect_id  { -1 };
    int invoice_id  { -1 };
    tickect_id = item->text(1).toInt();
    auto ticket_state = item->text(5);

    if ( ticket_state == "cancelado" ) return;

    if (ticket_state == "cobrado"){ // charged
        ticket_db->connect();
        invoice_id = ticket_db->getNextInvoiceVal();
        ticket_db->disConnect();

        if (invoice_id < 0) return;

        ticket_db->connect();
        bool is_ticket_invoiced = ticket_db->groupInvoiceWithTicket(tickect_id, invoice_id);
        ticket_db->disConnect();

        if (!is_ticket_invoiced){
            ticket_db->connect();
            ticket_db->setNextInvoiceVal(invoice_id);
            ticket_db->disConnect();
            return;
        }
    }
    else if ( ticket_state == "facturado" ){ //invoiced
        ticket_db->connect();
        invoice_id = ticket_db->getInvoiceFromTicket(tickect_id);
        ticket_db->disConnect();

        if (invoice_id < 0) return;
    }

    if ( tickect_id >= 0 && invoice_id >=0 ){
        ticket_db->connect();
        std::unique_ptr<XmlConfig> xml { ticket_db->getTicketFromDatabase(tickect_id) };
        ticket_db->disConnect();

        xml->delDomain();
        xml->createElement("invoicenumber", QString::number(invoice_id));

        PrinterManager printer(nullptr, "InvoicePrinter");
        printer.printTicket(xml.get(), 2);
        refreshList();
    }
}
