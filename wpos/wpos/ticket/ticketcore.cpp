/***************************************************************************
                          ticketcore.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "ticketcore.h"
#include "database/ticketcoredb.h"
#include "database/barcoredb.h"
#include "database/changemoneywidgetdb.h"

#include "printer/printermanager.h"
#include <wposcore/utility.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>

#include <xmlconfig.h>

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QMessageBox>

using namespace std;

#define TICKET_TMP "/tmp/ticketcore_tmp.xml"

TicketCore::TicketCore(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    db = new TicketCoreDB("TicketCoreConnection", cfg::xmlFileByKey(cfg::XMLKey::Database));
    db->connect();
    printer = new PrinterManager(this, "ticketPrinter");

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::X, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::Z, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG, this);

    gsm->subscribeToGenericSignal(GSIGNAL::X, this);
    gsm->subscribeToGenericSignal(GSIGNAL::Z, this);

    gsm->subscribeToGenericDataSignal(GDATASIGNAL::TICKETCORE_PRINTING_STATE,this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::TICKETCORE_PRINTING_COPIES,this);
}

TicketCore::~TicketCore(){
    db->disConnect();
    delete printer;
}

bool TicketCore::getTogglePrinterState(){
    return printer_state;
}

void TicketCore::togglePrinterState(bool on){
    printer_state = on;
}

void TicketCore::xSlot(){
    dailyCashOperations();
}

void TicketCore::zSlot(){
    dailyCashOperations(false);
}

void TicketCore::receiveCoreData(XmlConfig *_xml){
    if (!_xml)   return;

    XmlConfig xml;
    xml.readXmlFromString (_xml->toString() );

    auto ticket_number = db->getNextVal();
    if (ticket_number == -1)  return;
    xml.doWrite("ticketnumber", QString::number(ticket_number));

    if (!stampTicket(&xml)){
        cout << "Ticket Insertion Transaction Failed" << endl;
        cout << xml.toString().toStdString() << endl;
        return;
    }
    if (getTogglePrinterState()){
        //print ticket code.
    }
}

bool TicketCore::stampTicket(XmlConfig *xml){
    bool numeric_bool;
    bool ret;
    int ticket_number;
    int item_code;

    double total=0;
    QString string_tn;


    xml->pushDomain();
    xml->delDomain();
    string_tn = xml->readString("ticketnumber");
    if (string_tn.isEmpty()) return false;

    ticket_number = string_tn.toInt(&numeric_bool);
    if (!numeric_bool)   return false;

    //fill the ticket data and location;
    TicketData data;
    TicketLocationData location;

    //ticket data
    data.ticket_number = ticket_number;
    data.employee_id = xml->readString("employee.dni");
    data.start_time = xml->readString("timestamps.start");
    data.end_time = xml->readString("timestamps.last");
    data.pay_type = xml->readString("paytype");
    data.ticket_state = "cobrado"; // FIXME The set of states of the ticket hasn't been defined...

    location.lounge = xml->readString("location.lounge");
    location.table = xml->readString("location.table");
    location.ticket_number = ticket_number;

    //start the transaction trying with queries over the ticket_table and location_table
    db->startTransaction();
    if (!db->insertTicket(&data)){
        printf ("INSERT TICKET FAILED\n");
        db->rollback();
        db->setVal(ticket_number);
        xml->popDomain();
        return false;
    }

    //     if (!location.lounge.isEmpty()){
    if ( !db->insertLocation(&location) ){
        db->rollback();
        db->setVal(ticket_number);
        xml->popDomain();
        return false;
    }
    //     }

    xml->setDomain("products");
    for( auto i=0; i < xml->howManyTags("product"); ++i){
        if ( (item_code = db->getNextItemVal()) == -1 ) continue;
        xml->setDomain("product["+QString::number(i)+"]");
        TicketItemData itemData;
        itemData.item_code = item_code;
        itemData.ticket_number = ticket_number;
        itemData.product_code = getProductCode(xml);
        itemData.price = xml->readString("billing.price").toDouble();
        total += itemData.price;

        if (itemData.price < 0) itemData.units=-1;
        else itemData.units = 1;

        itemData.order_time = xml->readString("timestamps.ordertime");
        itemData.tax = xml->readAttribute("billing.tax","type");
        itemData.tax_rate = xml->readString("billing.tax").toDouble();

        if (!db->insertTicketItem(&itemData)){
            db->rollback();
            db->setVal(ticket_number);
            xml->popDomain();
            return false;
        }

        if (xml->setDomain("offer")){
            OfferData offer;
            offer.offer_type = xml->readString("type");
            offer.offer_name = xml->readString("name");
            offer.item_code = item_code;
            if (!db->insertOffer(&offer)){
                db->rollback();
                db->setVal(ticket_number);
                xml->popDomain();
                return false;
            }
            xml->releaseDomain("offer");
        }

        for (auto j=0; j < xml->howManyTags("option","options"); j++){
            TicketItemOptionData option;
            option.item_code = item_code;
            option.option_type = xml->readString("options.option["+QString::number(j)+"].type");
            option.option = xml->readString("options.option["+QString::number(j)+"].value");
            if (!db->insertItemOption(&option)){
                db->rollback();
                db->setVal(ticket_number);
                xml->popDomain();
                return false;
            }
        }
        xml->releaseDomain("product");
    }

    ret = db->commit();

    QString ticket_log;
    if ( total > 0 ) ticket_log = "ticket_add";
    else  ticket_log = "ticket_substract";

    if ( data.pay_type == "metalico"){
        XmlConfig xml;
        xml.createElement("event_type", ticket_log);
        xml.createElement("cash_quantity",QString::number(total));
        xml.createElement("ticket_number",QString::number(ticket_number));
        emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml);
    }

    xml->delDomain();
    xml->popDomain();

    if (printer_state)  printer->printTicket(*xml, num_copy);
    return ret;
}

QString TicketCore::getProductCode(XmlConfig *xml){
    QString sort_aux;
    //check the name at the list of articles
    xml->pushDomain();
    for ( auto i=0; i < xml->howManyTags("article","articles"); i++)
        sort_aux+= xml->readString("articles.article["+QString::number(i)+"].name") + " ";

    xml->popDomain();
    return sort_aux.simplified();
}

bool TicketCore::dailyCashOperations(bool partial){
    XmlConfig tmp_xml;
    tmp_xml.delDomain();

    //the timestamp for the X or Z actions
    QString last_timestamp = currentDateTimeString();
    tmp_xml.createElement("timestamp", last_timestamp);

    //timestamps of the first and last tickets at the database(at TICKETS table).
    QString first_timestamp = db->getLastZDate();
    if ( first_timestamp.isEmpty() ) first_timestamp = currentDateTimeString();;

    tmp_xml.createElement("timestamps.first", first_timestamp);
    tmp_xml.createElement("timestamps.last", last_timestamp);


    if ( !db->getXEmployeeData(&tmp_xml) ){
        cout << __FILE__ << endl;
        cout << __PRETTY_FUNCTION__ << " : " << __LINE__ - 2 << endl;
        cout << "Problems Obtaining  staff data" << endl;
        return false;
    }
    if ( !db->getXMain(&tmp_xml) ){
        cout << "Problems obtaining main data" << endl;
        return false;
    }
    if(!db->getXOfferSection(&tmp_xml)){
        cout << "Problems obtaining offer data " << endl;
        return false;
    }
    if(!db->getXOptionSection(&tmp_xml)){
        cout << "Problems obtaining options data " << endl;
        return false;
    }


    //there is a new section at the xml that with the cash at the cashbox
    ChangeMoneyWidgetDB tmp_db("TicketCoreCashInWidget", cfg::xmlFileByKey(cfg::XMLKey::Database));
    tmp_db.connect();
    tmp_xml.createElement("money_at_cashbox", QString::number(tmp_db.getMoneyInCash(),'f',2));
    tmp_db.disConnect();

    /*************************************************************
All this code will be moved soon to a new section called statistics which
will get all the statistics from the database... Also de TicketDB code will be moved
to this new section.
**************************************************************/
    //     QStringList product_list;
    //     product_list.prepend("2x9");
    //     //test of products
    //     db->getXProducts(xml,product_list);
    //     //insert 0 price products... (FIXME when the stock schema is finished)
    //     db->getXProductZeroPriced(xml);


    cout << "****************************************************************************" <<endl;
    cout << "                     THIS IS THE OUTPUT OF THE X or Z                      "  <<endl;
    cout << "****************************************************************************" <<endl;
    tmp_xml.debug();
    cout << "****************************************************************************" <<endl;

    //this is the X method
    if (partial){
        emit genericDataSignal(GDATASIGNAL::X, &tmp_xml);
        if (printer_state){
            //send the xml to the printer
            printer->printX(tmp_xml);
#ifndef _MSC_VER
            usleep(100);
#endif
            printer->printTicketTotal(tmp_xml);
        }

        XmlConfig xml_event;
        xml_event.createElement("event_type", "x");
        emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml_event);
    }
    else{ //this should be done at the Z method only and implies critical database operations
        //fill gaps
        XmlConfig xml_event;
        xml_event.createElement("event_type", "z");
        emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml_event);
        fillTicketGaps();
        if ( !db->vaccum() ){
            cout << "ZETA error"<< endl;
            return false;
        }
        emit genericDataSignal(GDATASIGNAL::Z, &tmp_xml);
        if (printer_state){
            printer->printZ(tmp_xml);
#ifndef _MSC_VER
            usleep(100);
#endif
            printer->printTicketTotal(tmp_xml);
        }
    }
    return true;
}

void TicketCore::genericSignalSlot(const QString& signal){
    if ( signal == GSIGNAL::X ) xSlot();
    else if ( signal == GSIGNAL::Z ) zSlot();
}

void TicketCore::genericDataSignalSlot(
        const QString& signal,
        XmlConfig *xml)
{
    QString tmp;
    if ( signal == GDATASIGNAL::TICKETCORE_PRINTING_STATE )
    {
        xml->delDomain();
        tmp = xml->readString("state");
        if ( tmp == "on" ||  tmp == "ON" )
            togglePrinterState(true);
        else if (tmp == "off" || tmp == "OFF")
            togglePrinterState(false);
    }
    else if ( signal == GDATASIGNAL::TICKETCORE_PRINTING_COPIES )
    {
        bool ok{false};
        xml->delDomain();
        tmp = xml->readString("copies");
        auto copies = tmp.toInt(&ok);
        if (ok) setTicketCopies(copies);
    }
}

void TicketCore::fillTicketGaps(){
    QString date;

    int first = db->getFirstTicketCode();
    int last = db->getLastTicketCode();
    if (last <= first)  return;

    for(auto i = first; i < last; i++){
        //check ticketcode gaps
        if (!db->ticketCodeExists(i)){
            if (date.isEmpty()){
                date = db->dateFromTicketCode(i+1);
                if (date.isEmpty())
                    date = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);               
            }
            TicketData data;
            data.ticket_number = i;
            data.employee_id = "1"; //the system user should be always "1"
            data.start_time = date;
            data.end_time = date;
            data.pay_type = "metalico";  //pay_type always at the database :?
            data.ticket_state = "cobrado";

            if (!db->insertTicket(&data)){
                db->rollback();
                continue;
            }
        }
        else  date = db->dateFromTicketCode(i);
    }
    db->setVal(last);
}

void TicketCore::setTicketCopies(uint copies){
    if ( copies > 0 && copies < 100 )
        num_copy = copies;
}
