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

namespace std{}
using namespace std;



#define TICKET_TMP "/tmp/ticketcore_tmp.xml"

TicketCore::TicketCore(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    db = new TicketCoreDB("TicketCoreConnection", Files::configFilePath("database"));
    db->connect();
    printer = new PrinterManager(this,"ticketPrinter");

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::X,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::Z,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG,this);
    gsm->subscribeToGenericSignal(GSIGNAL::X,this);
    gsm->subscribeToGenericSignal(GSIGNAL::Z,this);
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

void TicketCore::receiveCoreDataSlot(XmlConfig *_xml){

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

bool TicketCore::stampTicket(XmlConfig *_xml){
    bool numeric_bool;
    bool ret;
    int ticket_number;
    int item_code;
    int count,i;
    int op_count,j;
    double total=0;
    QString string_tn;
    QString ticket_log;
    TicketData *data=0;
    TicketLocationData *location=0;
    TicketItemData *item=0;
    TicketItemOptionData *option=0;
    XmlConfig *event = 0;

    _xml->pushDomain();
    _xml->delDomain();
    string_tn = _xml->readString("ticketnumber");
    if (string_tn.isEmpty())
        return false;

    ticket_number = string_tn.toInt(&numeric_bool);
    if (!numeric_bool)
        return false;


    //fill the ticket data and location;
    data = new TicketData;
    location = new TicketLocationData;

    //ticket data
    data->ticket_number = ticket_number;
    data->employee_id = _xml->readString("employee.dni");
    data->start_time = _xml->readString("timestamps.start");
    data->end_time = _xml->readString("timestamps.last");
    data->pay_type = _xml->readString("paytype");
    // FIXME The set of states of the ticket hasn't been defined...
    data->ticket_state = "cobrado";

    //location data
    //      not used by now...

    location->lounge = _xml->readString("location.lounge");
    location->table = _xml->readString("location.table");
    location->ticket_number=ticket_number;


    //start the transaction trying with queries over the ticket_table and location_table
    db->startTransaction();
    if (!db->insertTicket(data)){
        printf ("INSERTTICKET FAILED\n");
        db->rollback();
        db->setVal(ticket_number);
        _xml->popDomain();
        delete data;
        delete location;
        return false;
    }

    //     if (!location->lounge.isEmpty()){
    if (!db->insertLocation(location)){
        db->rollback();
        db->setVal(ticket_number);
        _xml->popDomain();
        delete data;
        delete location;
        return false;
    }
    //     }

    _xml->setDomain("products");
    count = _xml->howManyTags("product");
    for(i=0;i<count;i++){
        if ((item_code = db->getNextItemVal()) ==-1)
            continue;
        _xml->setDomain("product["+QString::number(i)+"]");
        item = new TicketItemData;
        item->item_code = item_code;
        item->ticket_number = ticket_number;
        item->product_code = this->getProductCode(_xml);
        item->price = (_xml->readString("billing.price")).toDouble();
        total += item->price;
        if (item->price < 0)
            item->units=-1;
        else
            item->units=1;
        item->order_time = _xml->readString("timestamps.ordertime");
        item->tax = _xml->readAttribute("billing.tax","type");
        item->tax_rate = (_xml->readString("billing.tax")).toDouble();

        if (!db->insertTicketItem(item)){
            db->rollback();
            db->setVal(ticket_number);
            _xml->popDomain();
            delete item;
            delete data;
            delete location;
            return false;
        }

        if (_xml->setDomain("offer")){
            OfferData *offer = new OfferData;
            offer->offer_type = _xml->readString("type");
            offer->offer_name = _xml->readString("name");
            offer->item_code = item_code;
            if (!db->insertOffer(offer)){
                db->rollback();
                db->setVal(ticket_number);
                _xml->popDomain();
                delete item;
                delete offer;
                delete data;
                delete location;
                return false;
            }
            _xml->releaseDomain("offer");
            delete offer;
        }

        op_count = _xml->howManyTags("option","options");
        for (j=0;j<op_count;j++){
            option = new TicketItemOptionData;
            option->item_code = item_code;
            option->option_type = _xml->readString("options.option["+QString::number(j)+"].type");
            option->option = _xml->readString("options.option["+QString::number(j)+"].value");
            if (!db->insertItemOption(option)){
                db->rollback();
                db->setVal(ticket_number);
                _xml->popDomain();
                delete option;
                delete item;
                delete data;
                delete location;
                return false;
            }
            delete option;
        }
        _xml->releaseDomain("product");
        delete item;
    }

    ret= db->commit();

    if (total > 0)
        ticket_log = "ticket_add";
    else
        ticket_log = "ticket_substract";


    if (data->pay_type=="metalico"){
        event = new XmlConfig();
        event->createElement("event_type",ticket_log);
        event->createElement("cash_quantity",QString::number(total));
        event->createElement("ticket_number",QString::number(ticket_number));
        emit genericDataSignal(GDATASIGNAL::EVENTLOG,event);
        delete event;
    }

    _xml->delDomain();
    _xml->popDomain();

    delete data;
    delete location;

    if (this->printer_state)
        printer->printTicket(_xml,num_copy);

    return ret;
}


QString TicketCore::getProductCode(XmlConfig *_xml){
    int count,i;
    QString sort_aux;
    XmlConfig *product = 0;
    //check the name at the list of articles
    product = _xml;
    product->pushDomain();

    sort_aux = "";
    count = product->howManyTags("article","articles");
    for (i=0;i<count;i++){
        sort_aux+= product->readString("articles.article["+QString::number(i)+"].name");
        if (i!=(count-1))
            sort_aux+=" ";
    }
    sort_aux.simplified();

    product->popDomain();
    return sort_aux;
}

bool TicketCore::dailyCashOperations(bool partial){
    QString first_timestamp, last_timestamp;
    XmlConfig *tmp_xml = 0;
    ChangeMoneyWidgetDB *tmp_db = 0;

    tmp_xml = new XmlConfig();
    tmp_xml->delDomain();

    //the timestamp for the X or Z actions
    last_timestamp = currentDateTimeString();
    tmp_xml->createElement("timestamp", last_timestamp);

    //timestamps of the first and last tickets at the database(at TICKETS table).
    first_timestamp = db->getLastZDate();
    if ( first_timestamp.isEmpty() ) first_timestamp = currentDateTimeString();;

    tmp_xml->createElement("timestamps.first", first_timestamp);
    tmp_xml->createElement("timestamps.last", last_timestamp);


    if ( !db->getXEmployeeData(tmp_xml) ){
        cout << __FILE__ << endl;
        cout << __PRETTY_FUNCTION__ << " : " << __LINE__ - 2 << endl;
        cout << "Problems Obtaining  staff data" << endl;
        return false;
    }
    if ( !db->getXMain(tmp_xml) ){
        cout << "Problems obtaining main data" << endl;
        return false;
    }
    if(!db->getXOfferSection(tmp_xml)){
        cout << "Problems obtaining offer data " << endl;
        return false;
    }
    if(!db->getXOptionSection(tmp_xml)){
        cout << "Problems obtaining options data " << endl;
        return false;
    }


    //there is a new section at the xml that with the cash at the cashbox
    tmp_db = new ChangeMoneyWidgetDB("TicketCoreCashInWidget", Files::configFilePath("database"));
    tmp_db->connect();
    tmp_xml->createElement("money_at_cashbox", QString::number(tmp_db->getMoneyInCash(),'f',2));
    tmp_db->disConnect();
    delete tmp_db;  tmp_db=0;

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
    tmp_xml->debug();
    cout << "****************************************************************************" <<endl;

    //this is the X method
    if (partial){
        emit genericDataSignal(GDATASIGNAL::X, tmp_xml);
        if (printer_state){
            //send the xml to the printer
            printer->printX(tmp_xml);
            usleep(100);
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
        if ( !db->zeta() ){
            cout << "ZETA error"<< endl;
            delete tmp_xml;
            return false;
        }
        emit genericDataSignal(GDATASIGNAL::Z, tmp_xml);
        if (printer_state){
            printer->printZ(tmp_xml);
            usleep(100);
            printer->printTicketTotal(tmp_xml);
        }
    }
    delete tmp_xml;
    return true;
}

void TicketCore::genericSignalSlot(const QString& signal){
    if ( signal == GSIGNAL::X){
        xSlot();
    }else if (signal == GSIGNAL::Z){
        zSlot();
    }
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
    int first;
    int last;
    int actual;
    QString date ="";
    TicketData *data=0;

    first = db->getFirstTicketCode();
    last = db->getLastTicketCode();

    if (last<=first)
        return;

    for(actual=first;actual<last;actual++){
        //check ticketcode gaps
        if (!db->ticketCodeExists(actual)){
            if (date.isEmpty()){
                date = db->dateFromTicketCode(actual+1);
                if (date.isEmpty()){
                    //                    date = QDate::currentDate().toString("dd/MM/yyyy")+" ";
                    //                    date+=QTime::currentTime().toString("hh:mm:ss");
                    date = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
                }
            }
            data = new TicketData;
            data->ticket_number = actual;
            data->employee_id = "1"; //the system user should be always "1"
            data->start_time = date;
            data->end_time = date;
            data->pay_type = "metalico";  //pay_type always at the database :?
            data->ticket_state = "cobrado";

            if (!db->insertTicket(data)){
                db->rollback();
                delete data;
                continue;
            }
            delete data;

        }
        else
            date = db->dateFromTicketCode(actual);
    }
    db->setVal(last);
}


void TicketCore::setTicketCopies(uint copies){
    if ( copies > 0 && copies < 100 )
        num_copy = copies;
}
