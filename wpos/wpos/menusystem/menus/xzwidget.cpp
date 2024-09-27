/***************************************************************************
                          bslxzwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "xzwidget.h"
#include "salesscreen.h"

#include "printer/printermanager.h"

#include <xmlconfig.h>
#include <wposcore/genericsignalmanager.h>

#include <QtWebEngineWidgets/QWebEngineView>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QFile>

extern "C"{
#ifndef _MSC_VER
    #include <gdc.h>
    #include <gdchart.h>
    #include <gdcpie.h>
#endif
    #include <stdio.h>
    #include <stdlib.h>
}

using namespace std;

#define TMP_LAST_Z "/tmp/last_z.xml"

const QString &main_graph    = "/tmp/ntpv_x_main_graph.png";
const QString &waiters_graph = "/tmp/ntpv_x_waiter_graph.png";
const QString &offers_graph  = "/tmp/ntpv_x_offers_graph.png";
const QString &options_graph = "/tmp/ntpv_x_options_graph.png";

XZWidget::XZWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    down_button->setIcon(QPixmap("controls:down_48.png"));
    up_button->setIcon(QPixmap("controls:up_48.png"));
    print_button->setIcon(QPixmap("controls:printer2.png"));
    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::Z, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::X, this);

    // Init printer

    // Prepare de browser to display X or Z html
    initBrowser();

    html_page  = "<html>";
    html_page += "<head>";
    {
        html_page += "<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">";
        html_page += "<meta http-equiv=\"CACHE-CONTROL\" content=\"no-cache\">";
        html_page += "<meta http-equiv=\"pragma\" content=\"no-cache\">";
        html_page += "<meta http-equiv=\"expires\" content=\"-1\">";

        html_page +="<title>";
        {
            if ( operation == OperationMode::XOperation)
                html_page+= tr("Temporary Activity Breakdown X") ;
            else
                html_page+= tr("Activity Breakdown Z") ;
        }
        html_page+="</title>";
    }
    html_page+="</head>";

    html_page+="<body>";

    // Prepare xmls needed to generic signal data connections
    up_button->setAutoRepeat(true);
    down_button->setAutoRepeat(true);

    connect(down_button, &QPushButton::released, this, &XZWidget::scrollDownSlot);
    connect(up_button, &QPushButton::released,this, &XZWidget::scrollUpSlot);
    connect(print_button, &QPushButton::released, this, &XZWidget::printSlot);
    connect(cancel_button, &QPushButton::released, this, &XZWidget::returnToADMmenuSlot);
}

bool XZWidget::initBrowser(){
    delete browser;
    delete b_layout;

    b_layout = qobject_cast<QVBoxLayout*>( browser_frame->layout());
    browser = new QWebEngineView(browser_frame);
//@benes    browser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//@benes    browser->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    b_layout = new QVBoxLayout(browser_frame);
    b_layout->addWidget(browser);
    browser->show();

    return true;
}

void XZWidget::scrollDownSlot(){
    browser->scroll( 0, 40);
}

void XZWidget::scrollUpSlot(){
    browser->scroll( 0, -40);
}

void XZWidget::xSlot(XmlConfig *xml){
    // Clear last x page
    html_page.clear();
    xml->pushDomain();
    xml->delDomain();
    xml->save(TMP_LAST_Z);

    QString head = tr("X from BAR BOX To Date ");
    head += xml->readString("timestamp");

    xz_header_label->setText(head);
    html_page += mainSection(xml);
    html_page += waitersSection(xml);
    html_page += offerSection(xml);
    html_page += optionSection(xml);
    html_page += productSection(xml);

    html_page +="</body></html>";

    initBrowser();
    browser->setHtml(html_page, QUrl("file://"));
    xml->popDomain();
}

void XZWidget::zSlot(XmlConfig *xml){

    xml->pushDomain();
    xml->delDomain();

    QString head = tr("Z from BAR BOX To Date");
    xml->save(TMP_LAST_Z);
    head += xml->readString("timestamp");

    xz_header_label->setText(head);

    // Clear last z page
    html_page.clear();
    html_page += mainSection(xml);
    html_page += waitersSection(xml);
    html_page += offerSection(xml);
    html_page += optionSection(xml);
    html_page += productSection(xml);
    html_page +="</body></html>";

    initBrowser();
    browser->setHtml(html_page, QUrl("file://"));
    xml->popDomain();
}

void XZWidget::printSlot(){

    if (!QFile(TMP_LAST_Z).exists())
        return;

    XmlConfig last_xml(TMP_LAST_Z);
    PrinterManager printer;
    switch(operation){
    case XOperation:
        printer.printX(last_xml);
        printer.printTicketTotal(last_xml);
        break;
    case ZOperation:
        printer.printZ(last_xml);
        printer.printTicketTotal(last_xml);
        break;
    }

}

void XZWidget::returnToADMmenuSlot(){
    XmlConfig xml;
    xml.createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void XZWidget::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    XmlConfig tmp_xml;
    tmp_xml.createElement("name", SalesScreen::XZ_MENU);
    if (signal_name == GDATASIGNAL::X){
        operation = XOperation;
        xSlot(xml);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &tmp_xml);
    }
    else if (signal_name == GDATASIGNAL::Z){
        operation = ZOperation;
        zSlot(xml);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &tmp_xml);
    }
}

/******************************************************************************************
*               HTML COMPOSER METHODS... ALL OF THIS SHOULD BE DONE BY
*               A XLS AND XLST... OR BE SHOWN WITH THE PDF GENERATED BY THE REPORT MANAGER
*******************************************************************************************/
QString XZWidget::mainSection(XmlConfig *xml){
    int i, count;
    QString head = "";
    QString sub_head = "";
    QString ret = "";
    QString montante,aux;
    double met_tot = 0.0;

    createMainGraph(xml, main_graph);

    head  ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+="<tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("CASH DETAILS")+"</b>";
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
    for(i=0; i< count; i++){
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

QString XZWidget::productSection(XmlConfig *xml){
    int count,i;
    QString head = "";
    QString product = "";
    QString ret="";

    head  = "<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+=" <tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("DESGLOSE DE PRODUCTOS")+"</b>";
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

QString XZWidget::waitersSection(XmlConfig *xml){
    int count, i, offer_count,option_count, j, partial;
    QString head;
    QString employee;
    QString option_section;
    QString offer;
    QString option;
    QString ret;

    createWaitersGraph(xml, waiters_graph.toLatin1());

    head ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    head+="<tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("STAFF DETAILS")+"</b></font>";
    head+="</td></tr></tbody></table><br>";
    head+="<IMG src=\""+waiters_graph+"\" align=\"right\" hspace=\"12\"border=\"0\">";

    employee ="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"70%\"><tbody>";
    employee+="<tr valign=\"middle\"><td width=\"25\"><hr noshadow style=\"border: solid black 1px;\"></td>";
    employee+="<td width=\"200\" style=\"border: solid black 1px;\"><b>#NAME#</b></td>";
    employee+="<td width=\"60%\"><hr noshadow style=\"border: solid black 1px;\"></td></tr>";
    employee+="</tbody></table>";
    employee+="<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\"><tbody>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("NIF")+": #ID#</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Tickets Collected")+": #NUMTICKETS#</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Total")+": #TOTAL# &#8364;</td></tr>";
    employee+="<tr><td width=\"40\"></td><td>"+tr("Cash Box Openings")+": #OPEN_CASHBOX# </td></tr>";
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

QString XZWidget::offerSection(XmlConfig *xml){
    int count, i;
    int partial = 0;
    QString head = "";
    QString offer_head ="";
    QString offer_feet="";
    QString offer="";
    QString ret="";

    if (!xml)
        return ret;

    xml->pushDomain();
    xml->delDomain();

    if (!xml->setDomain("offers"))
        return ret;
    count = xml->howManyTags("offer");

    if (count)
        createOffersGraph(xml,offers_graph);

    head+="<div align=\"left\"><table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"100%\">";
    head+=" <tbody><tr bgcolor=#\"3366ff\"><td><font size=\"+2\" color=\"#ffffff\"><b>"+tr("INVITATIONS AND OFFERS")+"</b>";
    head+="</font></td></tr></tbody></table><br>";

    head+="<table align=\"left\" cellpadding=\"2\" cellspacing=\"2\" border=\"0\" width=\"60%\"><tbody>";
    head+="<tr><td width=\"25\"></td><td><b><font size=\"+1\">"+tr("TOTAL INVITATIONS")+": #TOTALOFFERS#</font></b></td></tr>";
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

QString XZWidget::optionSection(XmlConfig *xml){
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
        createOptionsGraph(xml,options_graph);

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

bool XZWidget::createMainGraph(XmlConfig *xml, QString file_path, int width, int height){
    FILE *fp = 0;
    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    QFile file (file_path);
    if (file.exists()) file.remove();


    fp = fopen( file_path.toLatin1() , "wb" );
    if ( !fp || !xml )  return false;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("main");
    auto count = xml->howManyTags("paytype");

    values =(float *) malloc(count*sizeof(float));
    t =(char **) malloc(count*sizeof(char *));
#ifdef _GDC_H
    bzero(values,count*sizeof(float));
    bzero(t,count*sizeof(char*));
#endif

    for(auto i=0; i<count; i++){
        aux = xml->readString("paytype["+QString::number(i)+"].total_income");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        aux = xml->readString("paytype["+QString::number(i)+"].name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero( t[i], aux.length()+1 );
        strncpy( t[i], aux.toLatin1(), aux.length());
#endif
    }
#ifdef _GDC_H
    out_graph(width, height, fp, GDC_3DBAR,count, t, 1, values);
#endif

    for (auto i=0; i< count; i++)
        free(t[i]);

    free(t);
    free(values);

    fclose(fp);
    xml->popDomain();

    return true;
}

bool XZWidget::createWaitersGraph(XmlConfig *xml, QString file_path, int width, int height){

    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    QFile file (file_path);
    if (file.exists()) file.remove();


    FILE *fp = fopen( file_path.toLatin1() , "wb" );
    if ( !fp || !xml )  return false;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("employees");
    auto count = xml->howManyTags("employee");

    values =(float *) malloc( count*sizeof(float));
    t = (char**) malloc( count*sizeof(char*) );
#ifdef _GDC_H
    bzero(values, count*sizeof(float));
    bzero(t, count*sizeof(char*));
#endif

    if (!count){
        free(t);
        free(values);
        fclose(fp);
        xml->delDomain();
        xml->popDomain();
        return false;
    }

    for(auto i=0; i < count; i++){
        xml->setDomain("employee["+QString::number(i)+"]");
        aux = xml->readString("name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i],aux.length()+1);
        strncpy(t[i],aux.toLatin1(),aux.length());
#endif
        aux = xml->readString("total_income");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        xml->releaseDomain("employee");
    }
#ifdef _GDC_H
    out_graph(width, height, fp, GDC_3DBAR, count, t, 1, values);
#endif

    for (auto i=0; i < count; i++)
        free(t[i]);

    free(t);
    free(values);

    if (fclose(fp))
        perror("failed to close file descriptor");

    xml->delDomain();
    xml->popDomain();

    return true;
}

bool XZWidget::createOffersGraph(XmlConfig *xml,QString file_path,int width, int height){
    FILE *fp = 0;

    QString aux;
    float aux_float;
    float *values = 0;
    char **t = 0;
    unsigned long   sc[1]    = { 0x8080FF };
#ifdef _GDC_H
    GDC_BGColor   = 0xFFFFFFL;                  /* backgound color (white) */
    GDC_LineColor = 0x000000L;                  /* line color      (black) */
    GDC_SetColor  = &(sc[0]);                   /* assign set colors */
    GDC_stack_type = GDC_STACK_BESIDE;
#endif

    QFile file (file_path);
    if (file.exists()) file.remove();


    fp = fopen( file_path.toLatin1() , "wb" );
    if ( !fp || !xml )  return false;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("offers");
    auto count = xml->howManyTags("offer");

    values =(float *) malloc( count*sizeof(float));
    t =(char **) malloc( count*sizeof(char *));
#ifdef _GDC_H
    bzero(values, count*sizeof(float));
    bzero(t, count*sizeof(char*));
#endif

    if (!count){
        free(t);
        free(values);
        fclose(fp);
        xml->delDomain();
        xml->popDomain();
        return false;
    }

    for(auto i = 0; i < count; i++){
        aux = xml->readString("offer["+QString::number(i)+"].count");
        aux_float = aux.toFloat();
        values[i] = aux_float;
        aux = xml->readString("offer["+QString::number(i)+"].type");
        aux+=" " + xml->readString("offer["+QString::number(i)+"].name");
        t[i] = (char *) malloc((aux.length()+1)*sizeof(char));
#ifdef _GDC_H
        bzero(t[i], aux.length() + 1);
        strncpy( t[i], aux.toLatin1(), aux.length());
#endif
    }
#ifdef _GDC_H
    out_graph(width, height,fp,GDC_3DBAR,count,t,1,values);
#endif

    for (auto i=0; i < count; i++)
        free(t[i]);

    free(t);
    free(values);

    fclose(fp);
    xml->delDomain();
    xml->popDomain();

    return true;
}

bool XZWidget::createOptionsGraph(XmlConfig *xml,QString file_path,int width, int height){
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

    f = new QFile(file_path);
    if (f->exists())
        f->remove();
    delete f;

    fp = fopen( file_path.toLatin1() , "wb" );
    if (!fp)
        return false;

    if (!xml)
        return false;

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
        strncpy(t[i],aux.toLatin1(),aux.length());
#endif
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
