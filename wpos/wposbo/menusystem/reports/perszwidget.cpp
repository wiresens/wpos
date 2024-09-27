/***************************************************************************
                          perszwidget.cpp  -  description
                             -------------------
    begin                : mar may 11 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "perszwidget.h"

#include <wposgui/common/toolkit.h>
#include <libbslxml/xmlconfig.h>


#include <QtWebEngineWidgets/QWebEngineView>
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
#include <QStackedWidget>

#include <QProgressBar>
#include <QTimer>

#include <QLocale>

extern "C"{
#ifndef _MSC_VER
    #include <gdc.h>
    #include <gdchart.h>
    #include <gdcpie.h>
#endif
#include <stdio.h>
#include <stdlib.h>
}

#include <iostream>
using namespace std;
const uint TIMER_TIME = 2;

const uint PersZWidget::DEFAULT_WIDTH{200};
const uint PersZWidget::DEFAULT_HEIGHT{250};

PersZWidget::PersZWidget(QWidget *parent, const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    timer = new QTimer(this);
    setupButtons();
    setupSections();
    connectSignals();
//    initBrowser();
}

void PersZWidget::setupButtons(){
    first_z_button->setIcon(QPixmap("controls32:2uparrow.png"));
    up_z_button->setIcon(QPixmap("controls32:up.png"));
    down_z_button->setIcon(QPixmap("controls32:down.png"));
    last_z_button->setIcon(QPixmap("controls32:2downarrow.png"));
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    print_button->setIcon(QPixmap("controls32:printer2.png"));
    down_button->setIcon(QPixmap("controls48:down.png"));
    up_button->setIcon(QPixmap("controls48:up.png"));
    exit_button->setIcon(QPixmap("controls48:button_cancel.png"));
}

void PersZWidget::setupSections(){
    z_treeview->setHeaderLabels(headerLabels);
    z_treeview->setAllColumnsShowFocus(true);
    z_treeview->setColumnWidth(Icon, IconSize);
    z_treeview->setColumnWidth(StartDate, DateSize);
    z_treeview->setColumnWidth(EndDate, DateSize);
    z_treeview->setColumnWidth(Amount, AmountSize);
    z_treeview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    z_treeview->setSelectionMode(QListView::SingleSelection);
}

void PersZWidget::connectSignals(){
    connect(down_z_button, &QPushButton::clicked, this, &PersZWidget::downTicketSlot);
    connect(up_z_button, &QPushButton::clicked, this, &PersZWidget::upTicketSlot);

    connect(down_button, &QPushButton::clicked, this, &PersZWidget::downBrowserSlot);
    connect(up_button,&QPushButton::clicked, this, &PersZWidget::upBrowserSlot);

    connect(first_z_button, &QPushButton::clicked,this, &PersZWidget::firstTicketSlot);
    connect(last_z_button, &QPushButton::clicked, this, &PersZWidget::lastTicketSlot);

    connect(z_treeview, &QTreeWidget::itemSelectionChanged, this, &PersZWidget::zSelectedSlot);

    connect(start_datetime_dtedit, &QDateTimeEdit::dateTimeChanged, this, &PersZWidget::tableClickedSlot);
    connect(start_datetime_dtedit, &QDateTimeEdit::dateTimeChanged, this, &PersZWidget::setEndDateRange);
    connect(end_datetime_dtedit, &QDateTimeEdit::dateTimeChanged, this, &PersZWidget::tableClickedSlot);

    connect(print_button, &QPushButton::clicked, this, &PersZWidget::printSlot);
    connect(exit_button, &QPushButton::clicked, this, &PersZWidget::exitSlot);
    connect(ok_button, &QPushButton::clicked, this, &PersZWidget::acceptSlot);

    connect(timer, &QTimer::timeout, this, &PersZWidget::updateProgressBar);
}

void PersZWidget::showEvent(QShowEvent *e){
    stack->setCurrentWidget(main_page);
    clear();
    // getZ();
    QWidget::showEvent(e);
}

void PersZWidget::hideEvent(QHideEvent *e){
    stack->setCurrentWidget(main_page);
    clear();
    QWidget::hideEvent(e);
}

bool PersZWidget::isPrintServerAvailable() const{
//    DCOPClient *client = kapp->dcopClient();
//    client->attach();
//    return client->isAttached() && client->isApplicationRegistered("dcopprinter");
    return false;
}

void PersZWidget::clear(){
    z_treeview->clear();
    start_datetime_dtedit->setDateTime(QDateTime::currentDateTime().addSecs( -60*60));
    end_datetime_dtedit->setDateTime(QDateTime::currentDateTime());
    ok_button->setEnabled(checkAllValues());
}

void PersZWidget::upTicketSlot(){
    selectTreeItemUp(z_treeview);
}

void PersZWidget::downTicketSlot(){
    selectTreeItemDown(z_treeview);
}

void PersZWidget::firstTicketSlot(){
    selectTreeItemFirst(z_treeview);
}

void PersZWidget::lastTicketSlot(){
    selectTreeItemLast(z_treeview);
}

void PersZWidget::zSelectedSlot(){
    ok_button->setEnabled(checkAllValues());
}

void PersZWidget::tableClickedSlot(const QDateTime& start){
    startGetingZ();
    getZ(start, end_datetime_dtedit->dateTime());
    stopGettingZ();
}

void PersZWidget::setEndDateRange(const QDateTime& start){
    end_datetime_dtedit->setDateTimeRange(start, QDateTime::currentDateTime());
}

void PersZWidget::startGetingZ(){
    stack->setCurrentWidget(progress_page);
    progress_step = 0;
    progress_bar->setValue(0);
    timer->start(TIMER_TIME);
    qApp->processEvents();
}

void PersZWidget::getZ(const QDateTime& start, const QDateTime& end){
//    QDateTime init_date = QDateTime::currentDateTime();
//    init_date.setTime(QTime(0,0));
//    QDateTime finish_date = QDateTime::currentDateTime();
//    finish_date.setTime(QTime(23,59,59));
    double aux;
    bool aux_bool=false;

    z_treeview->clear();
    database.connect();
    QList<ZResumeData*> *resumes = database.getZResume(start.toString(), end.toString());
    database.disConnect();

    if (!resumes) return;

    for (const auto* data : *resumes){
        auto item = new QTreeWidgetItem(z_treeview);
        item->setIcon(Icon, QPixmap("controls:view_text.png"));
        item->setText(StartDate, data->start_date);
        item->setText(EndDate, data->end_date);
        (aux = data->cash.toDouble(&aux_bool));
        if (aux_bool)
//            item->setText(Amount, QString::number(aux,'f',2)+" "+tr(" Euros")); //@benes
            item->setText(Amount, QString::number(aux,'f',2)+" "+tr(" XAF")); //@benes
        item->setText(EventCode, data->event_code);
    }

    resumes->clear();
    delete resumes;
    ok_button->setEnabled(checkAllValues());
}

void PersZWidget::stopGettingZ(){
    timer->stop();
    stack->setCurrentWidget(main_page);
    qApp->processEvents();
}

void PersZWidget::updateProgressBar(){
    progress_step++;
    progress_bar->setValue(progress_step);
    qApp->processEvents();
}

bool PersZWidget::checkAllValues(){
    return  !z_treeview->selectedItems().isEmpty();
}

void PersZWidget::downBrowserSlot(){
//@benes    browser->scroll(0,150);
}

void PersZWidget::upBrowserSlot(){
//@benes     browser->scroll(0,-150);
}

void PersZWidget::acceptSlot(){
    stack->setCurrentWidget(viewer_page);
    XmlConfig xml;
    getCurrentZ(xml);
    if(xml.wellFormed()) showHtml(xmlToHtml(&xml));
}

void PersZWidget::exitSlot(){
    delete browser;
    browser = 0;
    delete b_layout;
    b_layout = 0;

    stack->setCurrentWidget(main_page);
    QDateTime start = QDateTime::currentDateTime();
    start.setTime(QTime(0,0));
    QDateTime end = QDateTime::currentDateTime();
    end.setTime(QTime(23,59,59));
    startGetingZ();
    getZ(start, end);
    stopGettingZ();
}

//void PersZWidget::printSlot(){
//    if (!xml) return;
//    printZ();
//    printTicketTotal();
//}

void PersZWidget::printSlot(){
    XmlConfig xml;
    getCurrentZ(xml);
    if ( xml.wellFormed() ){
        auto xml_str = xml.toString();
        printZ(xml_str);
        printTicketTotal(xml_str);
    }
}

void PersZWidget::printZ(const QString& xml_str){
   sendToPrintDeamon(xml_str);
}

void PersZWidget::sendToPrintDeamon(const QString& xml_str){
    if( !isPrintServerAvailable() ) return;
//    DCOPClient *client = client->attach();
//    QByteArray data;
//    QDataStream stream(&data, QIODevice::ReadWrite);
//    stream << xml_str;

//    if ( !client->send("dcopprinter", "printer", "printZ(QString)", data) )
//        cerr << "Problems while trying to call dcopprinter" << endl;
}

void PersZWidget::showHtml(const QString& html_str){
    initBrowser();
    browser->setHtml(html_str, QUrl("file://"));
}

void PersZWidget::printTicketTotal(const QString& xml_str){
    callPrintDeamon(xml_str);
}

void PersZWidget::callPrintDeamon(const QString& xml_str){
    if( !isPrintServerAvailable() ) return;
//    DCOPClient *client = client->attach();

//    QString reply_type;
//    QByteArray data, reply_data;
//    QDataStream out_stream(&data, QIODevice::ReadWrite);
//    out_stream << xml_str;

//    if ( !client->call("dcopprinter","printer","printTicketTotal(QString)", data, reply_type, reply_data))
//        cerr << "Problems while trying to call dcopprinter" << endl;

}

void PersZWidget::getCurrentZ(XmlConfig& xml){
    QTreeWidgetItem *item = z_treeview->currentItem();
    xml.createElement("timestamps.first", item->text(StartDate));
    xml.createElement("timestamps.last", item->text(EndDate));
    database.connect();

    auto noError = true;
    noError = database.createTempZ(item->text(EventCode));

    noError &= database.getXEmployeeData(&xml);
    if (!noError)
        cout << "Problems obtaining  staff data" << endl;

    noError &= database.getXMain(&xml);
    if ( !noError )
        cout << "Problems obtaining main data" << endl;

    noError &= database.getXOfferSection(&xml);
    if( !noError )
        cout << "Problems obtaining offer data " << endl;

    noError &= database.getXOptionSection(&xml);
    if( !noError )
        cout << "Problems obtaining option data " << endl;

    if( !noError){
        clear();
        stack->setCurrentWidget(main_page);
    }
    else
    {
        auto cash = QString::number(database.getMoneyInCash(),'f', 2);
        xml.createElement("money_at_cashbox", cash);
    }
    database.disConnect();
}

QString PersZWidget::xmlToHtml(XmlConfig *xml){
    if (!xml) QString();

    xml->pushDomain();
    xml->delDomain();

    QString html_page = "<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">";
    html_page += "<meta http-equiv=\"CACHE-CONTROL\" content=\"no-cache\">";
    html_page += "<meta http-equiv=\"Pragma\" content=\"no-cache\">";
    html_page += "<META HTTP-EQUIV=\"Expires\" CONTENT=\"-1\">";
    html_page +="<title>";
    html_page+= tr("Z Activity Breakdown") ;
    html_page+="</title></head><body>";
    html_page += mainSection(xml)+"<br>";
    html_page += waitersSection(xml)+"<br>";
    html_page += offerSection(xml)+"<br>";
    html_page += optionSection(xml)+"<br>";
    html_page +="</body></html>";

    xml->popDomain();
    return html_page;
}

//bool PersZWidget::initBrowser(){
//    delete browser;
//    delete b_layout;

//    browser=0;
//    b_layout=0;

//    b_layout = (QVBoxLayout *) browser_frame->layout();
//    browser = new Browser(browser_frame);

//    browser->setEditable(false);
//    browser->stopAnimations();
//    browser->setJScriptEnabled(false);
//    browser->setMetaRefreshEnabled(false);
//    browser->setDNDEnabled(false);
//    browser->setJavaEnabled(false);
//    browser->setPluginsEnabled(false);
//    browser->setAutoloadImages(false);
//    browser->setOnlyLocalReferences(true);

//    browser->view()->setVScrollBarMode(QScrollView::AlwaysOff);
//    browser->view()->setHScrollBarMode(QScrollView::AlwaysOff);
//    b_layout = new QVBoxLayout(browser_frame);
//    b_layout->addWidget(browser->view());
//    browser->view()->show();

//    return true;
//}

bool PersZWidget::initBrowser(){
    delete browser;
    delete b_layout;

    browser = new QWebEngineView(browser_frame);

//    browser->setEditable(false);
//    browser->stopAnimations();
//    browser->setJScriptEnabled(false);
//    browser->setMetaRefreshEnabled(false);
//    browser->setDNDEnabled(false);
//    browser->setJavaEnabled(false);
//    browser->setPluginsEnabled(false);
//    browser->setAutoloadImages(false);
//    browser->setOnlyLocalReferences(true);

//@benes    browser->view()->setVScrollBarMode(QScrollView::AlwaysOff);
//@benes    browser->view()->setHScrollBarMode(QScrollView::AlwaysOff);
    b_layout = qobject_cast<QVBoxLayout*>( browser_frame->layout());
    if(! b_layout) b_layout = new QVBoxLayout(browser_frame);
    b_layout->addWidget(browser);
    browser->show();
    return true;
}

QString PersZWidget::mainSection(XmlConfig *xml){
    int i, count;
    QString head = "";
    QString sub_head = "";
    QString foot_head = "";
    QString ret = "";
    QString montante,aux;
    double met_tot=0.0;

    createMainGraph(xml);

    head  ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+="<tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("DESGLOSE DE CAJA")+"</b>";
    head+="</font></td></tr></tbody></table><br>";
    head+="<IMG src=\""+main_graph+"\" hspace=\"12\"border=\"0\" align=\"right\">";
    head+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    head+="<tr><td width=\"25\"></td><td><b><font size=\"+2\">"+tr("TOTAL")+": #MAINTOTAL# &#8364;</font></b></td></tr>";
    head+="<tr><td width=\"25\"></td><td>"+tr("Amount in Cash Box")+": #MONTANTE# &#8364;</td></tr>";
    head+="<tr><td width=\"25\"></td><td>"+tr("Change")+": #CAMBIO# &#8364;</td></tr>";
    head+="</tbody></table><br>";

    sub_head =  "<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    sub_head+="<tbody><tr valign=\"middle\"><td width=\"25\">";
    sub_head+="<hr noshadow style=\"border: solid black 1px;\"></td>";
    sub_head+="<td width=\"100\" style=\"border: solid black 1px;\"><b>#PAYTYPE#</b></td>";
    sub_head+="<td width=\"80%\"><hr noshadow style=\"border: solid black 1px;\"></td></tr>";
    sub_head+="</tbody></table>";
    sub_head+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    sub_head+="<tbody><tr><td width=\"40\"></td>";
    sub_head+="<td>"+tr("Tickets")+": #NUMTICKETS#</td></tr>";
    sub_head+="<tr><td width=\"40\"></td><td>"+tr("Total")+": <b>#TOTAL# &#8364;</b></td>";
    sub_head+="</tr></tbody></table><br>";

    xml->pushDomain();
    xml->delDomain();
    montante = xml->readString("money_at_cashbox");
    xml->setDomain("main");
    ret += head;
    count = xml->howManyTags("paytype");
    for(i=0; i<count; i++){
        ret += sub_head;
        if ( xml->readString("paytype["+QString::number(i)+"].name")=="metalico" )
            met_tot = (xml->readString("paytype["+QString::number(i)+"].total_income")).toDouble();
        ret.replace("#PAYTYPE#",xml->readString("paytype["+QString::number(i)+"].name"));
        ret.replace("#NUMTICKETS#",xml->readString("paytype["+QString::number(i)+"].num_tickets"));
        ret.replace("#TOTAL#",xml->readString("paytype["+QString::number(i)+"].total_income"));
    }

    ret.replace("#MAINTOTAL#",xml->readString("total"));
    ret.replace("#MONTANTE#",montante);
    aux = QString::number((montante.toDouble() - met_tot),'f',2);
    ret.replace("#CAMBIO#",aux);

    ret+="<br>";

    xml->popDomain();
    return ret;
}

QString PersZWidget::productSection(XmlConfig *xml){
    int count,i;
    QString head;
    QString product;
    QString ret;

    head  = "<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+=" <tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("PRODUCT BREAKDOWN")+"</b>";
    head+="</font></td></tr></tbody></table><br>";

    product  = "<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody><tr valign=\"middle\">";
    product+="<td width=\"25\"><hr noshadow style=\"border: solid black 1px;\"></td>";
    product+="<td width=\"150\" style=\"border: solid black 1px;\"><b>#PRODUCT#</b></td>";
    product+="<td width=\"80%\"><hr noshadow style=\"border: solid black 1px;\"></td>";
    product+="</tr></tbody></table><table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    product+="<tbody><tr><td width=\"40\"></td><td>+"+tr("Sales")+": #UNITS#</td></tr>";
    product+="<tr><td width=\"40\"></td><td>"+tr("Total")+": #TOTAL# &#8364;</td></tr>";
    product+="</tbody></table><br>";

    xml->pushDomain();
    xml->delDomain();

    xml->setDomain("products");
    count = xml->howManyTags("product");
    if (count>=1)
        ret += head;
    for(i=0; i<count; i++){
        ret+= product;
        ret.replace("#PRODUCT#",xml->readString("product["+QString::number(i)+"].name"));
        ret.replace("#UNITS#",xml->readString("product["+QString::number(i)+"].units"));
        ret.replace("#TOTAL#",xml->readString("product["+QString::number(i)+"].total_income"));
    }
    xml->popDomain();

    ret+="<br>";

    return ret;
}

QString PersZWidget::waitersSection(XmlConfig *xml){
    int count, i, offer_count,option_count, j, partial;
    QString head;
    QString employee;
    QString employee_end;
    QString option_section;
    QString offer;
    QString option;
    QString ret;

    createWaitersGraph(xml);

    head ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    head+="<tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("STAFF BREAKDOWN")+"</b></font>";
    head+="</td></tr></tbody></table><br>";
    head+="<IMG src=\""+waiters_graph+"\" align=\"right\" hspace=\"12\"border=\"0\">";

    employee ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"70%\"><tbody>";
    employee+="<tr valign=\"middle\"><td width=\"25\"><hr noshadow style=\"border: solid black 1px;\"></td>";
    employee+="<td width=\"200\" style=\"border: solid black 1px;\"><b>#NAME#</b></td>";
    employee+="<td width=\"60%\"><hr noshadow style=\"border: solid black 1px;\"></td></tr>";
    employee+="</tbody></table>";
    employee+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("NIF")+": #ID#</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Tickets collected")+": #NUMTICKETS#</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Total")+": #TOTAL# &#8364;</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Cashbox Openings")+": #OPEN_CASHBOX# </td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Invitations")+": #TOTALOFFERS#</td></tr>";
    employee+="</tbody></table>";

    option_section+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    option_section+="<tr><td width=\"40\"></td><td>"+tr("Options")+":</td></tr>";
    option_section+="</tbody></table>";


    offer+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    offer+="<tr><td width=\"60\"></td><td>#OFFERTYPE#  [<b>#OFFERNAME#</b>] : #OFFERCOUNT#</td></tr>";
    offer+="</tbody></table>";

    option+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    option+="<tr><td width=\"60\"></td><td>#OPTIONTYPE#  [<b>#OPTIONNAME#</b>] : #OPTIONAMOUNT# &#8364;</td></tr>";
    option+="</tbody></table>";


    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("employees");

    ret+= head;

    count = xml->howManyTags("employee");
    for(i=0; i<count; i++){
        xml->setDomain("employee["+QString::number(i)+"]");
        partial = 0;
        ret+= employee;
        ret.replace("#NAME#",xml->readString("name"));
        ret.replace("#ID#",xml->readString("id"));
        ret.replace("#NUMTICKETS#",xml->readString("num_tickets"));
        ret.replace("#TOTAL#",xml->readString("total_income"));
        ret.replace("#OPEN_CASHBOX#",xml->readString("event_count"));
        if (xml->setDomain("offers")){
            offer_count = xml->howManyTags("offer");
            for(j=0;j<offer_count;j++){
                xml->setDomain("offer["+QString::number(j)+"]");
                ret += offer;
                ret.replace("#OFFERTYPE#",xml->readString("type"));
                ret.replace("#OFFERNAME#",xml->readString("name"));
                ret.replace("#OFFERCOUNT#",xml->readString("count"));
                partial += (xml->readString("count")).toInt();
                xml->releaseDomain("offer");
            }
            xml->releaseDomain("offers");
        }
        ret+=option_section;
        if (xml->setDomain("options")){
            option_count = xml->howManyTags("option");
            for(j=0;j<option_count;j++){
                xml->setDomain("option["+QString::number(j)+"]");
                ret += option;
                ret.replace("#OPTIONTYPE#",xml->readString("type"));
                ret.replace("#OPTIONNAME#",xml->readString("name"));
                //                                ret.replace("#OPTIONCOUNT#",xml->readString("count"));
                ret.replace("#OPTIONAMOUNT#",xml->readString("amount"));
                xml->releaseDomain("option");
            }
            xml->releaseDomain("options");
        }
        ret.replace("#TOTALOFFERS#",QString::number(partial));
        xml->releaseDomain("employee");
        ret+="<br>";
    }
    xml->popDomain();
    return ret;
}

QString PersZWidget::offerSection(XmlConfig *xml){
    int count, i;
    int partial = 0;
    QString head;
    QString offer_head;
    QString offer_feet;
    QString offer;
    QString ret;

    if (!xml)
        return ret;

    xml->pushDomain();
    xml->delDomain();

    if (!xml->setDomain("offers"))
        return ret;
    count = xml->howManyTags("offer");

    if (count)
        createOffersGraph(xml);


    head+="<div align=\"left\"><table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+=" <tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("INVITATIONS AND OFFERS")+"</b>";
    head+="</font></td></tr></tbody></table><br>";

    head+="<table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"60%\"><tbody>";
    head+="<tr><td width=\"25\"></td><td><b><font size=\"+1\">"+tr("TOTAL IN INVITATIONS")+": #TOTALOFFERS#</font></b></td></tr>";
    head+="</tbody></table><br>";

    offer_head+="<table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"50%\"><tbody>";
    offer+="<tr><td>#OFFERTYPE#</td>";
    offer+="<td>#OFFERNAME#</td>";
    offer+="<td>#OFFERCOUNT#</td></tr>";
    offer_feet+="</tbody></table></div>";


    if (count){
        ret += head;
        ret += offer_head;
    }
    for(i=0; i<count; i++){
        xml->setDomain("offer["+QString::number(i)+"]");
        ret += offer;
        ret.replace("#OFFERTYPE#",xml->readString("type"));
        ret.replace("#OFFERNAME#",xml->readString("name"));
        ret.replace("#OFFERCOUNT#",xml->readString("count"));
        partial += (xml->readString("count")).toInt();
        xml->releaseDomain("offer");

    }
    if (count){
        ret.replace("#TOTALOFFERS#",QString::number(partial));
        ret+= offer_feet;
        ret+="<IMG src=\""+offers_graph+"\" hspace=\"12\"border=\"0\" align=\"right\">";
    }


    ret+="<br><br>";
    xml->popDomain();
    return ret;
}

QString PersZWidget::optionSection(XmlConfig *xml){
    int count, i;
    QString head = "";
    QString offer_head ="";
    QString offer_feet="";
    QString offer="";
    QString ret="";


    if (!xml)
        return ret;

    xml->pushDomain();
    xml->delDomain();

    if (!xml->setDomain("options"))
        return ret;
    count = xml->howManyTags("option");

    if (count)
        createOptionsGraph(xml);

    head  = "<div align=\"left\"><table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+=" <tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("PRODUCT OPTIONS")+"</b>";
    head+="</font></td></tr></tbody></table>";


    offer_head+="<table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"50%\"><tbody>";
    offer+="<tr><td>#OPTION_TYPE#</td>";
    offer+="<td>#OPTION_NAME#</td>";
    //        offer+="<td>#OPTION_COUNT#</td>";
    offer+="<td>#OPTION_AMOUNT# &#8364;</td></tr>";
    offer_feet+="</tbody></table></div>";

    if (count){
        ret += head;
        ret += offer_head;
    }
    for(i=0; i<count; i++){
        xml->setDomain("option["+QString::number(i)+"]");
        ret += offer;
        ret.replace("#OPTION_TYPE#",xml->readString("type"));
        ret.replace("#OPTION_NAME#",xml->readString("name"));
        //                ret.replace("#OPTION_COUNT#",xml->readString("count"));
        ret.replace("#OPTION_AMOUNT#",xml->readString("amount"));
        xml->releaseDomain("option");
    }
    if (count){
        ret+="<IMG src=\""+options_graph+"\" hspace=\"12\"border=\"0\" align=\"right\">";
        ret+="<br><br>";
    }
    xml->popDomain();
    return ret;
}

bool PersZWidget::createMainGraph(XmlConfig *xml,int width, int height){
    FILE *fp = 0;
    QFile *f = 0;
    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
    int count,i;

#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    f = new QFile(main_graph);
    if (f->exists())
        f->remove();
    delete f;

    main_graph = "/tmp/ntpv_x_main_graph"+QString::number(rand()%65536)+".png";

    fp = fopen( main_graph.toLatin1() , "wb" );
    if (!fp)
        return false;

    if (!xml)
        return false;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("main");
    count = xml->howManyTags("paytype");

    values =(float *) malloc(count*sizeof(float));
    t =(char **) malloc(count*sizeof(char *));
#ifdef _GDC_H
    bzero(values,count*sizeof(float));
    bzero(t,count*sizeof(char*));
#endif

    for(i=0; i<count; i++){
        aux = xml->readString("paytype["+QString::number(i)+"].total_income");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        aux = xml->readString("paytype["+QString::number(i)+"].name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i],aux.length()+1);
#endif
        strncpy(t[i],aux.toLatin1(),aux.length());
    }
#ifdef _GDC_H
    out_graph(width, height,fp,GDC_3DBAR,count,t,1,values);
#endif

    for (i=0;i<count;i++){
        free(t[i]);
    }
    free(t);
    free(values);


    fclose(fp);
    xml->popDomain();

    return true;
}

bool PersZWidget::createWaitersGraph(XmlConfig *xml,int width, int height){
    FILE *fp = 0;
    QFile *f = 0;
    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
    int count=0;
    int i;
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    f = new QFile(waiters_graph);
    if (f->exists())
        f->remove();
    delete f;

    waiters_graph = "/tmp/ntpv_x_waiters_graph"+QString::number(rand()%65536)+".png";


    fp = fopen( waiters_graph.toLatin1() , "wb" );
    if (!fp)
        return false;

    if (!xml)
        return false;


    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("employees");
    count = xml->howManyTags("employee");

    values =(float *) malloc(count*sizeof(float));
    t =(char **) malloc(count*sizeof(char *));
#ifdef _GDC_H
    bzero(values,count*sizeof(float));
    bzero(t,count*sizeof(char*));
#endif
    if (!count){
        free(t);
        free(values);
        fclose(fp);
        xml->delDomain();
        xml->popDomain();
        return false;
    }

    for(i=0; i<count; i++){
        xml->setDomain("employee["+QString::number(i)+"]");
        aux = xml->readString("name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i],aux.length()+1);
#endif
        strncpy(t[i],aux.toLatin1(),aux.length());
        aux = xml->readString("total_income");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        xml->releaseDomain("employee");
    }

#ifdef _GDC_H
    out_graph(width, height,fp,GDC_3DBAR,count,t,1,values);
#endif

    for (i=0;i<count;i++){
        free(t[i]);
    }
    free(t);
    free(values);

    if (fclose(fp))
        perror("fallo al cerrar descriptor de fichero");

    xml->delDomain();
    xml->popDomain();

    return true;
}

bool PersZWidget::createOffersGraph(XmlConfig *xml,int width, int height){
    FILE *fp = 0;
    QFile *f = 0;
    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
    int count=0;
    int i;
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    f = new QFile(offers_graph);
    if (f->exists())
        f->remove();
    delete f;

    offers_graph = "/tmp/ntpv_x_offers_graph"+QString::number(rand()%65536)+".png";

    fp = fopen( offers_graph.toLatin1() , "wb" );
    if (!fp)
        return false;

    if (!xml)
        return false;


    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("offers");
    count = xml->howManyTags("offer");

    values =(float *) malloc(count*sizeof(float));
    t =(char **) malloc(count*sizeof(char *));
#ifdef _GDC_H
    bzero(values,count*sizeof(float));
    bzero(t,count*sizeof(char*));
#endif

    if (!count){
        free(t);
        free(values);
        fclose(fp);
        xml->delDomain();
        xml->popDomain();
        return false;
    }

    for(i=0; i<count; i++){
        aux = xml->readString("offer["+QString::number(i)+"].count");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        aux = xml->readString("offer["+QString::number(i)+"].type");
        aux+=" " + xml->readString("offer["+QString::number(i)+"].name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i],aux.length()+1);
#endif
        strncpy(t[i],aux.toLatin1(),aux.length());
    }
#ifdef _GDC_H
    out_graph(width, height,fp,GDC_3DBAR,count,t,1,values);
#endif

    for (i=0;i<count;i++){
        free(t[i]);
    }
    free(t);
    free(values);


    fclose(fp);
    xml->delDomain();
    xml->popDomain();

    return true;
}

bool PersZWidget::createOptionsGraph(XmlConfig *xml,int width, int height){
    FILE *fp = 0;
    QFile *f = 0;
    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
    int count=0,i=0;
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    f = new QFile(options_graph);
    if (f->exists()) f->remove();
    delete f;

    options_graph = "/tmp/ntpv_x_options_graph"+QString::number(rand()%65536)+".png";

    fp = fopen( options_graph.toLatin1() , "wb" );
    if (!fp) return false;
    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("options");
    count = xml->howManyTags("option");

    values =(float *) malloc(count*sizeof(float));
    t =(char **) malloc(count*sizeof(char *));
#ifdef _GDC_H
    bzero(values,count*sizeof(float));
    bzero(t,count*sizeof(char*));
#endif

    if (!count){
        free(t);
        free(values);
        fclose(fp);
        xml->delDomain();
        xml->popDomain();
        return false;
    }

    for(i=0; i<count; i++){
        aux = xml->readString("option["+QString::number(i)+"].amount");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        aux = xml->readString("option["+QString::number(i)+"].type");
        aux+=" " + xml->readString("option["+QString::number(i)+"].name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i],aux.length()+1);
#endif
        strncpy(t[i],aux.toLatin1(),aux.length());
    }
#ifdef _GDC_H
    out_graph(width, height,fp,GDC_3DBAR,count,t,1,values);
#endif

    for (i=0;i<count;i++){
        free(t[i]);
    }
    free(t);
    free(values);


    fclose(fp);
    xml->delDomain();
    xml->popDomain();

    return true;
}
