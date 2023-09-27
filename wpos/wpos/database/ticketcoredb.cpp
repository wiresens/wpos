/***************************************************************************
                          ticketcoredb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "ticketcoredb.h"
#include "barcoredb.h"
#include <xmlconfig.h>

#include <wposcore/utility.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QVariant>

TicketCoreDB::TicketCoreDB(const QString& _connection_name,
                           const QString& _hostname,
                           const QString& _database,
                           const QString& _username,
                           const QString& _passwd):
    BasicDatabase(_connection_name,_hostname, _database, _username,_passwd){}

TicketCoreDB::TicketCoreDB(const QString& _connection_name,
                           const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){}

TicketCoreDB::~TicketCoreDB(){}

int TicketCoreDB::getNextVal(){
    int ret = -1;

    if (!isConnected()) return ret;

    QString query {"SELECT nextval('tickets_ticket_code_seq');"};
    QSqlQuery query_obj {QSqlQuery(query, getDB())};

    QSqlError error{query_obj.lastError()};
    if ( error.type()!= QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return ret;
    }

    if (!query_obj.size()) return ret;

    query_obj.first();
    return query_obj.value(0).toInt();
}

/** set the value to the sequence at the database */
bool TicketCoreDB::setVal(int val){

    if ( val < 1 || !isConnected() ) return false;

    QString query {"SELECT setval('tickets_ticket_code_seq',"+QString::number(val)+",false);"};
    QSqlQuery query_obj  { QSqlQuery( query, getDB()) };

    QSqlError error{query_obj.lastError()};
    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

int TicketCoreDB::getNextItemVal(){

    int ret {-1};
    if ( !isConnected() ) return ret;

    QString query {"SELECT nextval('ticket_items_item_code_seq');"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };

    QSqlError error{query_obj.lastError()};
    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return ret;
    }

    query_obj.first();
    return query_obj.value(0).toInt();
}

/** set the value to the sequence at the database */
bool TicketCoreDB::setNextItemVal(int val){


    if ( val < 1 || !isConnected() ) return false;

    QString query {"SELECT setval('ticket_items_item_code_seq','"+QString::number(val)+"',false);"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };

    QSqlError error{query_obj.lastError()};
    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }

    return true;
}

bool TicketCoreDB::insertTicket(TicketData* data){

    if ( !isConnected() ) return false;

    QString query {"INSERT INTO tickets (ticket_code, employee_id, start_time, end_time, pay_type, ticket_state) "};
    query+= "VALUES (";
    query+= QString::number(data->ticket_number)+",";
    query+= "'"+data->employee_id+"',";
    query+= "'"+data->start_time+"',";
    query+= "'"+data->end_time+"',";
    query+= "'"+data->pay_type+"',";
    query+= "'"+data->ticket_state+"'";
    query+= ");";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool TicketCoreDB::insertLocation(TicketLocationData* data){

    if (!this->isConnected()) return false;

    QString query  {"INSERT INTO ticket_table (ticket_code, lounge_code,table_code) "};
    query+= "VALUES ("+QString::number(data->ticket_number)+",'"+data->lounge+"','"+data->table+"');";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool TicketCoreDB::insertTicketItem(TicketItemData* data){

    if ( !isConnected() ) return false;

    QString query {"INSERT INTO ticket_items (item_code, ticket_code, product_code, price, units, order_time, tax, tax_rate) "};
    query+= "VALUES (";
    query+= QString::number(data->item_code)+",";
    query+= QString::number(data->ticket_number)+",";
    query+= "'"+data->product_code+"',";
    query+= QString::number(data->price,'f',2)+",";
    query+= QString::number(data->units,'f',2)+",";
    query+= "'"+data->order_time+"',";
    query+= "'"+data->tax+"',";
    query+= QString::number(data->tax_rate,'f',2);
    query+= ");";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool TicketCoreDB::insertItemOption(TicketItemOptionData* data){

    if ( !isConnected() ) return false;

    QString query  {"INSERT INTO ticket_item_opts ( item_code, option_type, prod_option) "};
    query+= "VALUES (";
    query+= QString::number(data->item_code)+",";
    query+= "'"+data->option_type+"',";
    query+= "'"+data->option+"');";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool TicketCoreDB::insertOffer(OfferData* data){ 

    if ( !isConnected() ) return false;

    QString query {"INSERT INTO ticket_item_offers ( item_code, offer_type, prod_offer) "};
    query+= "VALUES (";
    query+= QString::number(data->item_code)+",";
    query+= "'"+data->offer_type+"',";
    query+= "'"+data->offer_name+"');";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }
    return true;
}

bool TicketCoreDB::getXEmployeeData(XmlConfig* xml){

    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();

    if ( !isConnected() ){
        xml->popDomain();
        return false;
    }

    QString sql {"SELECT employee, employee_id, num_tickets, total_income FROM x_employee;"};
    QSqlQuery query ( sql, getDB() );
    QSqlError error{query.lastError()};

    if ( !query.isActive() || error.type() != QSqlError::NoError || !query.size() ){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        xml->popDomain();
        return false;
    }

    int i{0};
    while ( query.next() ){
        xml->createElementSetDomain("employees.employee["+QString::number(i)+"]");
        auto employee_id = query.value(1).toString();
        xml->createElement("name", query.value(0).toString());
        xml->createElement("id", employee_id);
        xml->createElement("num_tickets", query.value(2).toString());
        auto aux_double = query.value(3).toDouble();
        xml->createElement("total_income", QString::number(aux_double,'f',2));
        getEmployeeProductInvitations(employee_id, xml);
        getEmployeeProductOptions(employee_id, xml);
        auto event_number = getEmployeeEvent(employee_id,"open_cashbox");
        xml->createElement("event_count[0]", QString::number(event_number));
        xml->createAttribute("event_count[0]","type","open_cashbox");
        xml->delDomain();
        i++;
    }
    xml->popDomain();
    return true;
}

int TicketCoreDB::getEmployeeProductInvitations(const QString& employee_id, XmlConfig* xml){

    if (! isConnected() ) return 0;

    QString query  {"SELECT offer_type, prod_offer,num_products "};
    query += "FROM x_employee_offers ";
    query += "WHERE employee_id='"+employee_id+"' order by offer_type;";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return 0;
    }

    xml->pushDomain();
    xml->createElementSetDomain("offers");
    int i = 0;
    while (query_obj.next()){
        xml->createElement("offer["+QString::number(i)+"].type", query_obj.value(0).toString());
        xml->createElement("offer["+QString::number(i)+"].name", query_obj.value(1).toString());
        xml->createElement("offer["+QString::number(i)+"].count", query_obj.value(2).toString());
        i++;
    }

    xml->releaseDomain("offers");
    xml->popDomain();
    return 0;
}

int TicketCoreDB::getEmployeeProductOptions(const QString& employee_id, XmlConfig* xml){

    int ret { 0 };
    if (! isConnected() ) return ret;

    QString query {"SELECT option_type, prod_option,num_products,total "};
    query += "FROM x_employee_options ";
    query += "WHERE employee_id='"+employee_id+"' order by option_type;";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return ret;
    }

    xml->pushDomain();
    xml->createElementSetDomain("options");

    int i {0};
    while (query_obj.next()){
        xml->createElement("option["+QString::number(i)+"].type", query_obj.value(0).toString() );
        xml->createElement("option["+QString::number(i)+"].name", query_obj.value(1).toString() );
        xml->createElement("option["+QString::number(i)+"].count", query_obj.value(2).toString() );
        xml->createElement("option["+QString::number(i)+"].amount", query_obj.value(3).toString() );
        i++;
    }
    xml->releaseDomain("options");
    xml->popDomain();
    return ret;
}

int TicketCoreDB::getEmployeeEvent(const QString& employee_id, const QString& event_type){

    int ret { 0 };
    if (! isConnected() ) return ret;

    QString query {"SELECT num_events FROM x_employee_events "};
    query += "WHERE (employee_id='"+employee_id+"') AND (event_type='"+event_type+"');";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return ret;
    }

    query_obj.first();
    return  query_obj.value(0).toInt();
}

bool TicketCoreDB::getXMain(XmlConfig* xml){

    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();

    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    QString query  {"SELECT pay_type, num_tickets, total_income FROM x_pay_type;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size() ){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        xml->popDomain();
        return false;
    }

    int j{0};
    double total{0.0};
    while (query_obj.next()){
        xml->createElementSetDomain("main.paytype["+QString::number(j)+"]");
        xml->createElement("name",(query_obj.value(0)).toString());
        xml->createElement("num_tickets",(query_obj.value(1)).toString());
        auto aux_double = (query_obj.value(2)).toDouble();
        xml->createElement("total_income",QString::number(aux_double,'f',2));
        total += aux_double;
        xml->delDomain();
        j++;
    }

    xml->createElement("main.total", QString::number(total,'f',2));
    xml->popDomain();
    return true;
}

bool TicketCoreDB::getXProducts(XmlConfig* xml, QStringList product_code_list){

    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();
    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    for (auto product_code : product_code_list){
        QString query  {"SELECT product, num_products, total_income FROM x_products "};
        query +="WHERE product_code='"+product_code+"';";

        QSqlQuery query_obj { QSqlQuery( query, getDB()) };
        if ( !query_obj.isActive() ){
            xml->popDomain();
            return false;
        }

        if ( !query_obj.size() )  continue;

        int j{0};
        while (query_obj.next()){
            xml->createElementSetDomain("products.product["+QString::number(j)+"]");
            xml->createElement("name", query_obj.value(0).toString());
            xml->createElement("units", query_obj.value(1).toString());
            auto aux_double =  query_obj.value(2).toDouble();
            xml->createElement("total_income", QString::number(aux_double,'f',2));
            xml->delDomain();
            j++;
        }
    }
    xml->popDomain();
    return true;
}

bool TicketCoreDB::getXOfferSection(XmlConfig* xml){

    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();
    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    QString query  {"SELECT offer_type, prod_offer, num_products FROM x_offers order by offer_type; "};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if (!query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        xml->popDomain();
        return false;
    }

    if (!query_obj.size()){
        xml->createElement("offers");
        return true;
    }

    int i {0};
    xml->createElementSetDomain("offers");
    while (query_obj.next()){
        xml->createElement("offer["+QString::number(i)+"].type", query_obj.value(0).toString());
        xml->createElement("offer["+QString::number(i)+"].name", query_obj.value(1).toString());
        xml->createElement("offer["+QString::number(i)+"].count", query_obj.value(2).toString());
        i++;
    }
    xml->delDomain();

    xml->popDomain();
    return true;
}

bool TicketCoreDB::getXOptionSection(XmlConfig* xml){

    if ( !isConnected() || !xml ) return false;
    xml->pushDomain();
    xml->delDomain();

    QString query {"SELECT option_type, prod_option, num_products, total FROM x_options order by option_type; "};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if (!query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        xml->popDomain();
        return false;
    }

    if (!query_obj.size()){
        xml->createElement("options");
        return true;
    }

    int i  {0};
    xml->createElementSetDomain("options");
    while (query_obj.next()){
        xml->createElement("option["+QString::number(i)+"].type", query_obj.value(0).toString());
        xml->createElement("option["+QString::number(i)+"].name", query_obj.value(1).toString());
        xml->createElement("option["+QString::number(i)+"].count", query_obj.value(2).toString());
        xml->createElement("option["+QString::number(i)+"].amount", query_obj.value(3).toString());
        i++;
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

bool TicketCoreDB::getXProductZeroPriced(XmlConfig* xml){

    if ( !isConnected() || !xml ) return false;

    xml->pushDomain();
    xml->delDomain();

    QString query {"SELECT product, num_products FROM x_product_invitations ;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size() ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        xml->popDomain();
        return false;
    }

    if (!xml->setDomain("products"))
        xml->createElementSetDomain("products");

    while (query_obj.next()){
        xml->createElementSetDomain("product");
        xml->createElement("name", query_obj.value(0).toString());
        xml->createElement("units", query_obj.value(1).toString());
        xml->createElement("total_income","0");
        xml->releaseDomain("product");
    }

    xml->delDomain();
    xml->popDomain();
    return true;
}

int TicketCoreDB::getFirstTicketCode(){

    if (!isConnected()) return -1;

    QString query {"SELECT DISTINCT MAX(ticket_code) FROM pers_tickets;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError ){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return -1;
    }

    if ( !query_obj.size() )
        return 1;

    query_obj.first();
    return  query_obj.value(0).toInt() + 1;
}

QString TicketCoreDB::getFirstTicketDate(){

    if (!isConnected())
        return QString("");

    QString query {"SELECT DISTINCT MIN(start_time) FROM tickets;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return QString("");
    }

    query_obj.first();
//    auto ret = query_obj.value(0).toDateTime().toString("dd/MM/yyyy hh:mm");
        auto ret = dateTimeString(query_obj.value(0).toDateTime());
    return ret;
}

int TicketCoreDB::getLastTicketCode(){

    if (!isConnected()) return -1;

    QString query {"SELECT DISTINCT MAX(ticket_code) FROM tickets;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return -1;
    }

    if (!query_obj.size()) return 1;

    query_obj.first();
    return  query_obj.value(0).toInt() + 1 ;
}

QString TicketCoreDB::getLastTicketDate(){

    if ( !isConnected() )  return QString();

    QString query {"SELECT DISTINCT MAX(end_time) FROM tickets;"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        return QString();
    }

    query_obj.last();
    return dateTimeString(query_obj.value(0).toDateTime());
}

QString TicketCoreDB::getLastZDate(){
    if ( !isConnected() )  return QString();

    QString query  {"SELECT time_stamp FROM pers_event_log WHERE event_code="};
    query += "(SELECT MAX(event_code) FROM pers_event_log WHERE event_type='z') ;";
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size() ){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        return QString();
    }

    query_obj.last();
    return dateTimeString(query_obj.value(0).toDateTime());
}

bool TicketCoreDB::ticketCodeExists(int ticket_code){

    if (!isConnected()) return false;

    QString query {"SELECT count(ticket_code) FROM tickets where ticket_code="+QString::number(ticket_code)+";"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{query_obj.lastError()};

    if ( !query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return false;
    }

    query_obj.first();
    bool ret = (bool)(query_obj.value(0)).toInt();

    return ret;
}

QString TicketCoreDB::dateFromTicketCode(int ticket_code){

    if ( isConnected() ) return QString("");

    QString query {"SELECT end_time FROM tickets where ticket_code="+QString::number(ticket_code)+";"};
    QSqlQuery query_obj { QSqlQuery( query, getDB()) };

    QSqlError error{ query_obj.lastError() };
    if ( !query_obj.isActive() || error.type() != QSqlError::NoError || !query_obj.size()){
        qDebug() << error.nativeErrorCode()+": "+ error.text();
        return QString("");
    }

    query_obj.first();
    return  query_obj.value(0).toString();
}

bool TicketCoreDB::zeta(){

    //be prepared to a lot of dirty code (DBCODE)
    if ( !startTransaction() ) return false;

    QString query {"INSERT INTO pers_tickets (ticket_code, employee_id, start_time, end_time, pay_type, pay_fare)"};
    query+= "SELECT t.ticket_code, t.employee_id, t.start_time, t.end_time, t.pay_type, p.pay_fare ";
    query+= "FROM tickets t, pay_types p ";
    query+= "WHERE t.pay_type = p.pay_type;";

    QSqlQuery query_obj { QSqlQuery( query, getDB()) };
    QSqlError error{ query_obj.lastError() };

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query = "INSERT INTO pers_ticket_table (ticket_code, table_code, lounge_code) ";
    query+= "SELECT t.ticket_code, t.table_code, t.lounge_code ";
    query+= "FROM ticket_table t;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "INSERT INTO pers_ticket_items (item_code, ticket_code, product_code, product, price, units, tax, tax_rate, order_time) ";
    query+= "SELECT t.item_code, t.ticket_code, t.product_code, p.product, t.price, t.units, t.tax, t.tax_rate, t.order_time ";
    query+= "FROM ticket_items t, products p ";
    query+= "WHERE t.product_code = p.product_code;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "INSERT INTO pers_ticket_item_offers (item_code, offer_type, prod_offer)";
    query+= "SELECT t.item_code, t.offer_type, t.prod_offer ";
    query+= "FROM ticket_item_offers t;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "INSERT INTO pers_ticket_item_opts (item_code, option_type, prod_option)";
    query+= "SELECT t.item_code, t.option_type, t.prod_option ";
    query+= "FROM ticket_item_opts t;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "INSERT INTO pers_event_log (event_code, employee_id, time_stamp, event_type) ";
    query+= "SELECT e.event_code, e.employee_id, e.time_stamp, e.event_type ";
    query+= "FROM event_log e;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "INSERT INTO pers_cash_movements (event_code, ticket_code, quantity) ";
    query+= "SELECT c.event_code, c.ticket_code, c.quantity ";
    query+= "FROM cash_movements c;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM ticket_item_opts;";

    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM ticket_items;";
    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM ticket_table;";
    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM cash_movements;";
    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM event_log;";
    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    query =  "DELETE FROM tickets;";
    query_obj =  QSqlQuery( query, getDB() );
    error = query_obj.lastError();

    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        rollback();
        return false;
    }

    bool ret = commit();

    query =  "VACUUM;";
    query_obj =  QSqlQuery( query, getDB() );
//    error = query_obj.lastError();

//    if (!query_obj.isActive() || error.type() != QSqlError::NoError){
//        qDebug() << error.nativeErrorCode()+": "+ error.text();
//        rollback();
//        return false;
//    }

    return ret;
}
