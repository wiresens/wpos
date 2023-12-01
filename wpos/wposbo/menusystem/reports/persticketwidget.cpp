/***************************************************************************
                          persticketwidget.cpp  -  description
                             -------------------
    begin                : mar may 11 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "persticketwidget.h"
#include <libbslxml/xmlconfig.h>



#include <wposwidget/orderview.h>
#include <wposwidget/ordercontentview.h>
#include <wposwidget/toolkit.h>
#include <wposcore/signals.h>

#include <QDataStream>
#include <QPushButton>
#include <QFrame>
#include <QLayout>
#include <QGroupBox>
#include <QListView>
#include <QPixmap>
#include <QLabel>
#include <QString>

#include <QStringList>
#include <qdatetime.h>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>

#include <QProgressBar>
#include <QTimer>
#include <QStackedWidget>
#include <QSplitter>

#include <QLocale>

#include <iostream>
using namespace std;

#define INIT_RES 0
#define TIMER_TIME 2

PersTicketWidget::PersTicketWidget(QWidget *parent, const QString& name):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    order_up_button->setVisible(false);
    order_down_button->setVisible(false);
    first_ticket_button->setVisible(false);
    last_ticket_button->setVisible(false);
    up_type_button->setVisible(false);
    down_type_button->setVisible(false);

    first_ticket_button->setIcon(QPixmap("controls32:2uparrow.png"));
    up_type_button->setIcon(QPixmap("controls32:up.png"));
    down_type_button->setIcon(QPixmap("controls32:down.png"));
    last_ticket_button->setIcon(QPixmap("controls32:2downarrow.png"));
    order_up_button->setIcon(QPixmap("controls48:up.png"));
    order_down_button->setIcon(QPixmap("controls48:down.png"));
    ok_button->setIcon(QPixmap("controls32:printer2.png"));

    // Create the connection to the database
    timer = new QTimer(this);

    //establish the listView Aspect
    ticketnum_listview->setHeaderLabels(QStringList{ tr("State"), tr("Code"), tr("Empoyee"), tr("Date")});
    ticketnum_listview->setAllColumnsShowFocus(true);
    ticketnum_listview->setColumnWidth(0,60);
    ticketnum_listview->setColumnWidth(1,100);
    ticketnum_listview->setColumnWidth(2,160);
    ticketnum_listview->setColumnWidth(3,120);
    ticketnum_listview->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    layoutItems();

    clear();
    //        getPersTickets();

    ticketnum_listview->sortByColumn(3);
    ticketnum_listview->setSelectionMode(QListView::SingleSelection);

    // Establish standart connections
    connect(this, &PersTicketWidget::genericDataSignal,
            order->orderContent(), &OrderContentView::genericDataSignalSlot);

    connect(ok_button, &QPushButton::clicked, order->orderContent(), &OrderContentView::printTicket);

    connect(down_type_button,  &QPushButton::clicked, this, &PersTicketWidget::downTicketSlot);
    connect(up_type_button,  &QPushButton::clicked, this, &PersTicketWidget::upTicketSlot);

    connect(first_ticket_button,  &QPushButton::clicked, this, &PersTicketWidget::firstTicketSlot);
    connect(last_ticket_button,  &QPushButton::clicked, this, &PersTicketWidget::lastTicketSlot);

    connect(order_down_button,   &QPushButton::clicked, order->orderContent(), &OrderContentView::selectDown);
    connect(order_up_button,   &QPushButton::clicked, order->orderContent(), &OrderContentView::selectUp);

    connect(ticketnum_listview, &QTreeWidget::itemClicked, this, &PersTicketWidget::ticketSelectedSlot);

    connect(init_datepicker, &QDateTimeEdit::dateTimeChanged, this, &PersTicketWidget::tableClickedSlot);
    connect(end_datepicker, &QDateTimeEdit::dateTimeChanged, this, &PersTicketWidget::tableClickedSlot);
    connect(timer, &QTimer::timeout, this, &PersTicketWidget::timerDone);

    stack->setCurrentWidget(main_page);
}

//void PersTicketWidget::layoutItems(){
//    // Create an instance of order and add it to this menu
//    order_main_layout->removeItem(nav_button_hlayout);

//    order = new BslOrderWidget(order_frame, "ORDER", 0);
//    QVBoxLayout *order_layout = new QVBoxLayout(order_frame);
//    order_layout->addWidget(order);
//    order_main_layout->addLayout(order_layout);
//    order_main_layout->addLayout( nav_button_hlayout);
//}

void PersTicketWidget::layoutItems(){
    // Create an instance of order and add it to this menu
    main_page_hlayout->removeItem(order_main_hlayout);
    main_page_hlayout->removeWidget(tickets_frame);

    order_main_layout->removeItem(nav_button_hlayout);

    QSplitter* splitter = new QSplitter(main_page);
    splitter->setChildrenCollapsible(false);
    main_page_hlayout->addWidget(splitter);

    auto tickets_frame_main_hlayout = new QHBoxLayout(main_page);
    tickets_frame_main_hlayout->addItem(tickets_frame_vlayout);

    splitter->addWidget(order_frame);
    splitter->addWidget(tickets_frame);

    order = new OrderView(order_frame, "ORDER");
    QVBoxLayout *order_layout = new QVBoxLayout(order_frame);
    order_layout->addWidget(order);
    order_main_layout->addLayout(order_layout);
    order_main_layout->addLayout( nav_button_hlayout);
}

PersTicketWidget::~PersTicketWidget(){
    ticketnum_listview->clear();
    delete order;
}

void PersTicketWidget::clear(){
    ticketnum_listview->clear();
    init_datepicker->setDate((QDate::currentDate()).addDays(-INIT_RES));
    end_datepicker->setDate(QDate::currentDate());
    clearOrder();
    ok_button->setEnabled(checkAllValues());
}

void PersTicketWidget::tableClickedSlot(){
    clearOrder();
    this->startGetingZ();
    getPersTickets();
    this->stopGettingZ();
}

void PersTicketWidget::getPersTickets(){

    QDate init_date = init_datepicker->date();
    QDate finish_date = end_datepicker->date();

    if (init_date > finish_date){
        QString text = tr("The dates entered are not correct.\en\The start date is earlier than the end date");
        QMessageBox::warning(this,tr("Can't get tickets"),text);
        return;
    }

    ticketnum_listview->clear();
    database.connect();
    QVector<TicketResumeData> resumes = database.getTicketResume(init_date.toString("dd/MM/yyyy")+" 00:00:00", finish_date.toString("dd/MM/yyyy")+" 23:59:59");
    database.disConnect();

    if (resumes.isEmpty()) return;

    for (const auto & data : resumes){
        auto item = new QTreeWidgetItem(ticketnum_listview);
        if (data.ticket_state == "cancelado")
            item->setIcon(0, anulation_pixmap);
        else if (data.ticket_state == "cobrado")
            item->setIcon(0, null_pixmap);

        item->setText(1, data.ticket_code);
        item->setText(2, data.employee_name);
        item->setText(3, data.stamp_time);
        item->setText(4, data.ticket_state);
    }
    ok_button->setEnabled(checkAllValues());
}

void PersTicketWidget::startGetingZ(){
    stack->setCurrentWidget(progress_page);
    progress_step = 0;
    progress_bar->setValue(0);
    timer->start(TIMER_TIME);
    qApp->processEvents();
}

void PersTicketWidget::stopGettingZ(){
    timer->stop();
    stack->setCurrentWidget(main_page);
    qApp->processEvents();
}

void PersTicketWidget::timerDone(){
    progress_step++;
    progress_bar->setValue(progress_step);
    qApp->processEvents();
}

void PersTicketWidget::showEvent(QShowEvent *e){
    clear();
    getPersTickets();
//    ticketnum_listview->scrollToItem(ticketnum_listview->lastItem());
    QWidget::showEvent(e);
}

void PersTicketWidget::clearOrder(){
    XmlConfig *aux_xml = 0;
    order->changeData (aux_xml = new XmlConfig ());
    delete aux_xml;
    ok_button->setEnabled(checkAllValues());
}

void PersTicketWidget::downTicketSlot(){
    selectTreeItemDown( ticketnum_listview);
}

void PersTicketWidget::upTicketSlot(){
    selectTreeItemUp( ticketnum_listview);
}

void PersTicketWidget::firstTicketSlot(){
    selectTreeItemFirst( ticketnum_listview);
}

void PersTicketWidget::lastTicketSlot(){
    selectTreeItemLast( ticketnum_listview);
}

void PersTicketWidget::ticketSelectedSlot(QTreeWidgetItem* item) {
    int tid=0;

    /* Get the ticket ID */
    if (!item) return;
    tid = item->text(1).toInt();

    {
        XmlConfig xml;
        if (item->text(4) == "cancelado")
            xml.createElement ("mode", "product_anulation");
        else
            xml.createElement ("mode", "normal");
        emit genericDataSignal ( GDATASIGNAL::SETCORE_MODE, &xml);
    }


    /* Get the ticket */
    database.connect();
    auto xml = database.getTicketFromDatabase(tid);
    database.disConnect();

    if (!xml)  return;
    order->changeData(xml);
    order->orderContent()->selectFirst();
    delete xml;
    ok_button->setEnabled(checkAllValues());
}

bool PersTicketWidget::checkAllValues(){

    auto items = ticketnum_listview->selectedItems();
    if (items.isEmpty()) return false;

    QDate init_date = init_datepicker->date();
    QDate finish_date = end_datepicker->date();

    if (init_date>finish_date)  return false;

    std::unique_ptr<XmlConfig> xml {order->orderContent()->getLocalXml()};
    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();
    if (!xml->setDomain("products")){
        xml->popDomain();
        return false;
    }

    if (!xml->howManyTags("product")){
        xml->popDomain();
        return false;
    }

    xml->popDomain();
//@benes    delete xml;

    return true;
}
