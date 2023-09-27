/***************************************************************************
                          bslkillticketsdb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "killticketsdb.h"
#include "authcoredb.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QDebug>

#include <xmlconfig.h>
#include <wposcore/utility.h>

KillTicketsDB::KillTicketsDB(
        const QString& _connection_name,
        const QString& _hostname,
        const QString& _database,
        const QString& _username,
        const QString& _passwd) :
    BasicDatabase(
        _connection_name,
        _hostname,
        _database,
        _username,
        _passwd)
{}

KillTicketsDB::KillTicketsDB(
        const QString& _connection_name,
        const QString& configuration_path) :
    BasicDatabase(_connection_name, configuration_path)
{}

KillTicketsDB::~KillTicketsDB(){}

int KillTicketsDB::getNextInvoiceVal(){

    int ret = -1;
    if (!isConnected()) return ret;

    QString query {"SELECT nextval('invoices_invoice_code_seq');"};
    QSqlQuery query_obj  {QSqlQuery(query,this->getDB())};
    if (!query_obj.isActive()){
        qDebug() << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ ;
        return ret;
    }

    if (!query_obj.size()){
        qDebug() << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ ;
        return ret;
    }
    else{
        query_obj.first();
        ret = query_obj.value(0).toInt();
    }
    return ret;
}

/** set the value to the sequence at the database */
bool KillTicketsDB::setNextInvoiceVal(int val){

    bool ret = false;

    if (val < 1)  return ret;

    if (!isConnected()) return ret;

    QString query {"SELECT setval('invoices_invoice_code_seq','"+QString::number(val)+"',false);"};
    QSqlQuery query_obj =  QSqlQuery(query,this->getDB());
    if (!query_obj.isActive()){
        qDebug() << "BslKillTicketsDB::setNextInvoiceVal() problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ ;
        return ret;
    }
    return true;
}

QList<TicketResumeData*>* KillTicketsDB::getTicketResume(){

    if (isConnected())
        connect();

    QString query  {"SELECT ticket_code, employee_name, employee_id, end_time, total, ticket_state "};
    query += "FROM get_tickets();";

    QSqlQuery query_obj {QSqlQuery(query, getDB())};
    if (!query_obj.isActive())
        return nullptr;

    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError)
        qDebug() << error.nativeErrorCode()+": "+ error.text();

    TicketResumeData *tikect_resume{};
    auto tikect_resume_list = new QList<TicketResumeData*>;
    while(query_obj.next()){
        tikect_resume = new TicketResumeData;
        tikect_resume->ticket_code = query_obj.value(0).toString();
        tikect_resume->employee_name = query_obj.value(1).toString();
        tikect_resume->employee_id = query_obj.value(2).toString();
        tikect_resume->timestamp = dateTimeString(query_obj.value(3).toDateTime(), false);
        tikect_resume->total = query_obj.value(4).toString();
        tikect_resume->ticket_state = query_obj.value(5).toString();
        tikect_resume_list->append(tikect_resume);
    }
    return tikect_resume_list;
}

QList<TicketResumeData*>* KillTicketsDB::getReceiptResume(QString employee_id){

    QList<TicketResumeData*> *tiket_resume_list{};

    if (!this->isConnected())
        return tiket_resume_list;

    QString query {"SELECT employee_id, start_time, blocked, name, description "};
    query += "FROM orders JOIN staff USING (employee_id) WHERE employee_id = '"+employee_id+"';";

    QSqlQuery query_obj =  QSqlQuery(query,this->getDB());

    if (!query_obj.isActive()) return tiket_resume_list;


    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError)
        qDebug() << error.nativeErrorCode()+": "+ error.text();

    tiket_resume_list = new QList<TicketResumeData*>;

    while(query_obj.next()){
        TicketResumeData *tiket_resume  {new TicketResumeData};
        tiket_resume->employee_id = (query_obj.value(0)).toString();
        tiket_resume->timestamp = (query_obj.value(1)).toString();
        tiket_resume->timestamp.replace("T"," ");
        tiket_resume->receipt_state = (query_obj.value(2)).toBool();
        tiket_resume->employee_name = (query_obj.value(3)).toString();
        tiket_resume->table_code = (query_obj.value(4)).toString();
        tiket_resume_list->append(tiket_resume);
    }

    return tiket_resume_list;
}

QList<TicketResumeData*>* KillTicketsDB::getReceiptResume(){

    QList<TicketResumeData*>* ticket_resume_list{nullptr};

    if (!this->isConnected())
        return ticket_resume_list;

    QString query  {"SELECT employee_id, start_time, blocked, name, description "};
    query += "FROM orders JOIN staff USING (employee_id);";
    //       query += "FROM orders GROUP BY employee_id ORDER BY employee_id,start_time ;";

    QSqlQuery query_obj =  QSqlQuery(query,this->getDB());
    if (!query_obj.isActive())
        return ticket_resume_list;


    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError)
        qDebug() << error.nativeErrorCode()+": "+ error.text();

    ticket_resume_list = new QList<TicketResumeData*>;
    while(query_obj.next()){
        TicketResumeData* ticket_resume = new TicketResumeData;
        ticket_resume->employee_id = query_obj.value(0).toString();
        ticket_resume->timestamp = query_obj.value(1).toString();
        ticket_resume->timestamp.replace("T"," ");
        ticket_resume->receipt_state = query_obj.value(2).toBool();
        ticket_resume->employee_name = query_obj.value(3).toString();
        ticket_resume->table_code = query_obj.value(4).toString();
        ticket_resume_list->append(ticket_resume);
    }

    return ticket_resume_list;
}

bool KillTicketsDB::ticketHasNegative (int ticket){

    if (!this->isConnected ())
        return true;

    QString query{ "SELECT price FROM ticket_items WHERE ticket_code = "+ QString::number(ticket)+";"};
    QSqlQuery query_obj =  QSqlQuery (query, this->getDB());

    if (!query_obj.isActive ()) return true;

    while (query_obj.next ()) {
        if (query_obj.value (0).toDouble () < 0)
            return true;
    }

    return false;
}

bool KillTicketsDB::ticketHasNegative (const QString& ticket) {
    return (ticketHasNegative(ticket.toInt()));
}

XmlConfig* KillTicketsDB::getTicketFromDatabase (int ticket_number){

    QString query {"SELECT e.name || ' ' || e.last_name, i.employee_id, i.start_time, i.end_time, "};
    query += " i.pay_type, t.lounge_code, t.table_code ";
    query += "FROM (tickets i JOIN staff e USING (employee_id)) LEFT JOIN ticket_table t USING ";
    query += "(ticket_code) WHERE ticket_code="+QString::number(ticket_number)+";";

    QSqlQuery query_obj =  QSqlQuery (query, this->getDB());

    if (!query_obj.isActive ())
        return nullptr;

    query_obj.first();
    auto employee = query_obj.value(0).toString();
    auto employee_id = query_obj.value(1).toString();
    auto start_time = query_obj.value(2).toDateTime();
    auto end_time = query_obj.value(3).toDateTime();
    auto pay_type = query_obj.value(4).toString();
    auto lounge_code = query_obj.value(5).toString();
    auto table_code = query_obj.value(6).toString();

    bool tab_ret = false;
    int tab_id = table_code.toInt(&tab_ret);

    XmlConfig* xml = new XmlConfig();
    if (tab_ret){
        if (tab_id > 0 )
            xml->createElement("description", lounge_code+" "+table_code);
        else if (tab_id == -1)
            xml->createElement("description", lounge_code);
    }

    xml->createElement ("location");
    xml->createElement ("location.lounge",lounge_code);
    xml->createElement ("location.table",table_code);

    xml->createElement ("timestamp");
//    xml->createElement ("timestamps.start", start_time.toString ("dd/MM/yyyy hh:mm:ss"));
//    xml->createElement ("timestamps.last", end_time.toString ("dd/MM/yyyy hh:mm:ss"));

    xml->createElement ("timestamps.start", dateTimeString(start_time));
    xml->createElement ("timestamps.last", dateTimeString(end_time));

    xml->createElement ("employee");
    xml->createElement ("employee.name", employee);
    xml->createElement ("employee.dni", employee_id);

    xml->createElement ("products");
    xml->createElement ("paytype", pay_type);
    xml->createElement ("ticketnumber", QString::number(ticket_number));

    /* Get the products */
    query = "SELECT product_code, price, units, order_time, tax, tax_rate, item_code FROM ticket_items WHERE ticket_code = ";
    query += QString::number (ticket_number) +";";

    query_obj.finish();
    query_obj =  QSqlQuery (query, this->getDB());

    if (!query_obj.isActive ()) {
        delete xml;
        return nullptr;
    }

    int product_i{0};
    while (query_obj.next() ) {
        auto product_code = query_obj.value(0).toString();
        auto price = query_obj.value(1).toString();
        auto units = query_obj.value(2).toString();
        auto order_time = query_obj.value(3).toDateTime();
        auto tax = query_obj.value(4).toString();
        auto tax_rate = query_obj.value(5).toString();
        auto item_code = query_obj.value(6).toString();

        xml->createElement("products.product");
        xml->setDomain("products.product["+QString::number (product_i)+"]");
        xml->createElement("timestamps");
//        xml->createElement("timestamps.ordertime", order_time.toString("dd/MM/yyyy hh:mm:ss"));
        xml->createElement("timestamps.ordertime", dateTimeString(order_time));

        /* Get the product name */
        QSqlQuery sub_query_obj {QSqlQuery ("SELECT product FROM products WHERE product_code = '"+product_code+"'", getDB())};
        if (!sub_query_obj.isActive ())
            continue;

        sub_query_obj.first ();
        QString product = sub_query_obj.value(0).toString ();

        xml->createElement("name", product);
        xml->createElement("billing");
        xml->createElement("billing.price", price);
        xml->createElement("billing.currency", "euro");
        xml->createElement("billing.tax", tax_rate);
        xml->createAttribute("billing.tax", "type", tax);

        xml->createElement("articles");

        sub_query_obj =  QSqlQuery ("SELECT ingredient_code FROM prod_composition WHERE product_code = '"+product_code+"'", this->getDB());

        if (!sub_query_obj.isActive ())
            continue;

        for (auto art_i = 0; sub_query_obj.next(); art_i++) {
            auto ingredient = sub_query_obj.value(0).toString();
            xml->createElement("articles.article["+QString::number (art_i)+"].name", ingredient);
        }

        sub_query_obj =  QSqlQuery("SELECT option_type, prod_option FROM ticket_item_opts WHERE item_code='"+item_code+"'",this->getDB());
        if (!sub_query_obj.isActive ())
            continue;

        if ( sub_query_obj.size() >0 )
            for (int art_i = 0; sub_query_obj.next (); art_i++) {
                auto option = sub_query_obj.value(0).toString();
                auto o_value = sub_query_obj.value(1).toString();
                xml->createElement("options.option["+QString::number (art_i)+"].type", option);
                xml->createElement("options.option["+QString::number (art_i)+"].value", o_value);
            }

        sub_query_obj =  QSqlQuery("SELECT offer_type, prod_offer FROM ticket_item_offers WHERE item_code='"+item_code+"'",this->getDB());
        if (!sub_query_obj.isActive ())
            continue;

        if (sub_query_obj.size() > 0){
            sub_query_obj.first();
            auto offer = sub_query_obj.value(0).toString();
            auto name = sub_query_obj.value(1).toString();
            xml->createElement("offer.type", offer);
            xml->createElement("offer.name", name);
        }
        xml->delDomain ();
        product_i++;
    }

    return xml;
}

int KillTicketsDB::getInvoiceFromTicket(int ticket){

    int ret = -1;
    if ( !isConnected() ) return ret;

    QString query {"SELECT invoice_code from invoices where ticket_code="+QString::number(ticket)+";"};
    QSqlQuery query_obj =  QSqlQuery(query,this->getDB());
    if (!query_obj.isActive())
        return ret;

    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return ret;
    }

    query_obj.first();
    return  query_obj.value(0).toInt();
}

bool KillTicketsDB::groupInvoiceWithTicket(int ticket_code, int invoice_code){

    bool ret = false;
    if (!isConnected()) return ret;

    startTransaction();
    QString query  {"UPDATE tickets SET ticket_state='facturado' WHERE ticket_code="+QString::number(ticket_code)+";"};
    QSqlQuery query_obj =  QSqlQuery(query, getDB());

    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        rollback();
        return ret;
    }

    query = "INSERT INTO invoices (invoice_code, moment, ticket_code) ";
    query+= "VALUES ("+QString::number(invoice_code)+",now(),"+QString::number(ticket_code)+");";

    query_obj =  QSqlQuery(query,this->getDB());
    error = query_obj.lastError();

    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        rollback();
        return ret;
    }
    ret = commit();
    return ret;
}
