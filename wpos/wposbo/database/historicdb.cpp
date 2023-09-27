/***************************************************************************
                          historicdb.cpp  -  description
                             -------------------
    begin                : mar may 11 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "historicdb.h"

#include <libbslxml/xmlconfig.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QDebug>

#include <iostream>

using namespace std;

HistoricDB::HistoricDB(const QString& _connection_name,
                       const QString& _hostname,
                       const QString& _database,
                       const QString& _username,
                       const QString& _passwd):
    BasicDatabase(_connection_name,
                  _hostname,
                  _database,
                  _username,
                  _passwd){}

HistoricDB::HistoricDB(const QString& _connection_name,
                       const QString& configuration_path):
    BasicDatabase(_connection_name,
                  configuration_path){}

HistoricDB::~HistoricDB(){}

QVector<TicketResumeData> HistoricDB::getTicketResume(
        QString start_date,
        QString end_date)
{

    if ( !isConnected() )  return QVector<TicketResumeData>();

    QString sql  =  "SELECT ticket_code, employee_name, employee_id, end_time, total, ticket_state ";
    sql += "FROM get_pers_tickets('"+start_date+"','"+end_date+"');";

    QSqlQuery query(sql, getDB());
    if (!query.isActive()) return QVector<TicketResumeData>();

    QSqlError error = query.lastError();
    if (error.type()!=QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::ConnectionError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< sql.toStdString() << endl;
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::TransactionError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::UnknownError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        }
    }

    QVector<TicketResumeData> resumes;
    while(query.next()){
        TicketResumeData data;
        data.ticket_code = query.value(0).toString();
        data.employee_name = query.value(1).toString();
        data.employee_id = query.value(2).toString();
        data.stamp_time = query.value(3).toDateTime().toString("dd/MM/yyyy hh:mm");
        data.total = query.value(4).toString();
        data.ticket_state = query.value(5).toString();
        resumes.push_back(data);
    }

    return resumes;
}

QList<ZResumeData*>* HistoricDB::getZResume(
        QString start_date,
        QString end_date)
{

    QList<ZResumeData*> *resumes = 0;
    ZResumeData *data = 0;
    double aux_float;

    if (!isConnected()) return nullptr;

    QString sql  =  "SELECT start_time, end_time, cash, event_code ";
//    sql += "FROM get_z('"+start_date+"','"+end_date+"');";
    sql += "FROM get_z();";


    QSqlQuery query(sql, getDB());
    if (!query.isActive()) return nullptr;

    QSqlError error { query.lastError()};
    if (error.type() != QSqlError::NoError){
        switch (error.type()){
        case  QSqlError::ConnectionError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::StatementError:
            cout << "QUERY ERROR: "<< sql.toStdString() << endl;
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::TransactionError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        case  QSqlError::UnknownError:
            cerr << "Database Connection Error in :" <<  __PRETTY_FUNCTION__ << ";" << __LINE__ << " "<< error.type()<< endl;
            break;
        }
    }

    resumes = new QList<ZResumeData*>;
    while(query.next()){
        data = new ZResumeData;
        data->start_date = ((query.value(0)).toDateTime()).toString("dd/MM/yyyy hh:mm");
        data->end_date = ((query.value(1)).toDateTime()).toString("dd/MM/yyyy hh:mm");
        aux_float = (query.value(2)).toDouble();
        data->cash = QString::number(aux_float,'f',2);
        data->cash = (query.value(2)).toString();
        data->event_code = (query.value(3)).toString();
        resumes->append(data);
    }
    return resumes;
}


XmlConfig *HistoricDB::getTicketFromDatabase (int ticket_number)
{
    QString sql = "SELECT e.name || ' ' || e.last_name, i.employee_id, i.start_time, i.end_time, ";
    sql += " i.pay_type, t.lounge_code, t.table_code ";
    sql += "FROM (pers_tickets i JOIN staff e USING (employee_id)) LEFT JOIN pers_ticket_table t USING ";
    sql += "(ticket_code) WHERE ticket_code=" + QString::number(ticket_number) + ";";

     QSqlQuery query(sql, getDB());
    if (!query.isActive ()) return nullptr;

    query.first();
    auto employee = query.value(0).toString();
    auto employee_id = query.value(1).toString();
    auto start_time = query.value(2).toDateTime();
    auto end_time = query.value (3).toDateTime();
    auto pay_type = query.value (4).toString();
    auto lounge_code = query.value(5).toString();
    auto table_code = query.value(6).toString();
    bool tab_ret= false;
    auto tab_id = table_code.toInt(&tab_ret);

    /* Get the products */
    sql = "SELECT product_code, price, units, order_time, tax, tax_rate, item_code FROM pers_ticket_items WHERE ticket_code = ";
    sql += QString::number (ticket_number) +";";

    query = QSqlQuery (sql, getDB());
    if (!query.isActive ()) return nullptr;

    XmlConfig *xml = new XmlConfig();
    if (tab_ret){
        if (tab_id > 0)  xml->createElement("description", lounge_code + " " + table_code);
        if (tab_id==-1)  xml->createElement("description", lounge_code);
    }

    xml->createElement ("location");
    /*
        *       FIXME
        *       Fix when box locations are correct
        */
    xml->createElement ("location.lounge", "barra");
    xml->createElement ("location.table");
    xml->createElement ("timestamp");
    xml->createElement ("timestamps.start", start_time.toString ("dd/MM/yyyy hh:mm:ss"));
    xml->createElement ("timestamps.last", end_time.toString ("dd/MM/yyyy hh:mm:ss"));
    xml->createElement ("employee");
    xml->createElement ("employee.name", employee);
    xml->createElement ("employee.dni", employee_id);
    xml->createElement ("products");
    xml->createElement ("paytype", pay_type);
    xml->createElement ("ticketnumber", QString::number(ticket_number));

    int product_i = 0;
    while (query.next ()) {
        auto product_code = query.value(0).toString();
        auto price = query.value(1).toString();
        auto units = query.value(2).toString();
        auto order_time = query.value(3).toDateTime();
        auto tax = query.value(4).toString();
        auto tax_rate = query.value(5).toString();
        auto item_code = query.value(6).toString();

        xml->createElement ("products.product");
        xml->setDomain ("products.product[" + QString::number(product_i) + "]");
        xml->createElement ("timestamps");
        xml->createElement ("timestamps.ordertime", order_time.toString ("dd/MM/yyyy hh:mm:ss"));

        /* Get the product name */
        QSqlQuery sub_query("SELECT product FROM products WHERE product_code = '"+product_code+"'", getDB());
        if (!sub_query.isActive ()) continue;

        sub_query.first();
        auto product = sub_query.value(0).toString();

        xml->createElement ("name", product);
        xml->createElement ("billing");
        xml->createElement ("billing.price", price);
        xml->createElement ("billing.currency", "euro");
        xml->createElement ("billing.tax", tax_rate);
        xml->createAttribute ("billing.tax", "type", tax);
        xml->createElement ("articles");

        sub_query = QSqlQuery ("SELECT ingredient_code FROM prod_composition WHERE product_code = '"+product_code+"'", getDB());

        if (!sub_query.isActive ())  continue;

        for (auto i = 0; sub_query.next (); i++) {
            auto ingredient = sub_query.value(0).toString ();
            xml->createElement ( "articles.article[" + QString::number(i) + "].name" , ingredient);
        }

        sub_query = QSqlQuery("SELECT option_type, prod_option FROM pers_ticket_item_opts WHERE item_code='"+item_code+"'",this->getDB());
        if (!sub_query.isActive ()) continue;

        if ( sub_query.size() > 0){
            for (auto i = 0; sub_query.next (); i++) {
                auto option = sub_query.value(0).toString();
                auto o_value = sub_query.value(1).toString();
                xml->createElement( "options.option[" + QString::number(i) + "].type" , option);
                xml->createElement( "options.option[" + QString::number(i) + "].value" , o_value);
            }
        }
        sub_query = QSqlQuery("SELECT offer_type, prod_offer FROM pers_ticket_item_offers WHERE item_code='"+item_code+"'",this->getDB());
        if (!sub_query.isActive ()) continue;

        if ( sub_query.size() > 0){
            sub_query.first();
            auto offer = sub_query.value(0).toString();
            auto name = sub_query.value(1).toString();
            xml->createElement("offer.type", offer);
            xml->createElement("offer.name", name);
        }
        xml->delDomain ();
        product_i++;
    }
    return xml;
}

double HistoricDB::getMoneyInCash ()
{
    if (!isConnected()) return -1;

    QString sql = "SELECT sum(quantity) FROM v_cash_movements;";
    QSqlQuery query(sql, getDB());

    if (!query.isActive()) return -1;

    query.first();
    return query.value(0).toDouble();
}

bool HistoricDB::createTempZ(const QString& code){
    if ( !isConnected() )  return false;

    QString sql = "SELECT create_z_view('"+code+"')";
    QSqlQuery query(sql, getDB());
    return query.isActive();
}

bool HistoricDB::getXEmployeeData(XmlConfig *xml)
{

    if (!xml)  return false;

    xml->pushDomain();
    xml->delDomain();

    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    QString sql = "SELECT employee, employee_id, num_tickets, total_income FROM z_employee;";
    QSqlQuery query(sql, getDB());
    QSqlError error{query.lastError()};
    if (!query.isActive() || error.type() != QSqlError::NoError || !query.size()){
        qDebug() << error.nativeErrorCode()+" : "+ error.text();
        xml->popDomain();
        return false;
    }

    int i{0};
    while (query.next()){
        xml->createElementSetDomain("employees.employee["+QString::number(i)+"]");
        auto employee_id = query.value(1).toString();
        xml->createElement("name", query.value(0).toString());
        xml->createElement("id", employee_id);
        xml->createElement("num_tickets", query.value(2).toString());
        auto aux_double = query.value(3).toDouble();
        xml->createElement("total_income", QString::number(aux_double,'f',2));
        getEmployeeProductInvitations(employee_id,xml);
        getEmployeeProductOptions(employee_id,xml);
        auto event_number = getEmployeeEvent(employee_id, "open_cashbox");
        xml->createElement("event_count[0]", QString::number(event_number));
        xml->createAttribute("event_count[0]", "type", "open_cashbox");
        xml->delDomain();
        i++;
    }

    xml->popDomain();
    return true;
}

int HistoricDB::getEmployeeProductInvitations(
        const QString& employee_id,
        XmlConfig *xml)
{
    if (!isConnected()) return 0;

    QString sql = "SELECT offer_type, prod_offer,num_products ";
    sql += "FROM z_employee_offers ";
    sql += "WHERE employee_id='"+employee_id+"' order by offer_type;";
    QSqlQuery query(sql, getDB());

    if (!query.isActive() || !query.size()) return 0;

    xml->pushDomain();
    xml->createElementSetDomain("offers");

    int i = 0;
    while (query.next()){
        xml->createElement( "offer[" + QString::number(i) + "].type" , query.value(0).toString() );
        xml->createElement( "offer[" + QString::number(i) + "].name" , query.value(1).toString() );
        xml->createElement( "offer[" + QString::number(i) + "].count" , query.value(2).toString() );
        i++;
    }

    xml->releaseDomain("offers");
    xml->popDomain();
    return 1;
}

int HistoricDB::getEmployeeProductOptions(const QString& employee_id,XmlConfig *xml){

    if (!isConnected()) return 0;

    QString sql = "SELECT option_type, prod_option,num_products,total ";
    sql += "FROM z_employee_options ";
    sql += "WHERE employee_id='"+employee_id+"' order by option_type;";

    QSqlQuery query(sql, getDB());

    if (!query.isActive() || !query.size()){
        cout << "Problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        return 0;
    }

    xml->pushDomain();
    xml->createElementSetDomain("options");

    int i = 0;
    while (query.next()){
        xml->createElement( "option[" + QString::number(i) + "].type" , query.value(0).toString());
        xml->createElement( "option[" + QString::number(i) + "].name" , query.value(1).toString());
        xml->createElement( "option[" + QString::number(i) + "].count" , query.value(2).toString());
        xml->createElement( "option[" + QString::number(i) + "].amount" , query.value(3).toString());
        i++;
    }

    xml->releaseDomain("options");
    xml->popDomain();
    return 1;
}

int HistoricDB::getEmployeeEvent(const QString& employee_id,const QString& event_type){

    int ret = 0;
    if (!isConnected()) return 0;
    QString sql =   "SELECT num_events FROM z_employee_events ";
    sql += "WHERE (employee_id='"+employee_id+"') AND (event_type='"+event_type+"');";

     QSqlQuery query(sql, getDB());
     if (!query.isActive() || !query.size()){
         cout << "Problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
         return 0;
     }

    query.first();
    return ret = query.value(0).toInt();
}

bool HistoricDB::getXMain(XmlConfig *xml){


    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();

    if (! isConnected()){
        xml->popDomain();
        return false;
    }

    QString sql = "SELECT pay_type, num_tickets, total_income FROM z_pay_type;";

    QSqlQuery query (sql, getDB());
    if (!query.isActive() || !query.size()){
        cout << "Problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        xml->popDomain();
        return false;
    }

    int i=0;
    double total=0;
    while (query.next()){
        xml->createElementSetDomain( "main.paytype[" + QString::number(i) + "]");
        xml->createElement("name", query.value(0).toString());
        xml->createElement("num_tickets", query.value(1).toString());
        auto aux_double = query.value(2).toDouble();
        xml->createElement("total_income", QString::number(aux_double,'f',2));
        total += aux_double;
        xml->delDomain();
        i++;
    }

    xml->createElement("main.total", QString::number(total,'f', 2));
    xml->popDomain();
    return true;
}

bool HistoricDB::getXOfferSection(XmlConfig *xml)
{
    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();

    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    QString sql = "SELECT offer_type, prod_offer, num_products FROM z_offers order by offer_type; ";
    QSqlQuery query(sql, getDB());

    if ( !query.isActive() ){
        xml->popDomain();     
        return false;
    }

    if ( !query.size() ){
        xml->createElement("offers");
        return true;
    }

    int i = 0;
    xml->createElementSetDomain("offers");
    while (query.next()){
        xml->createElement( "offer[" + QString::number(i) + "].type" ,query.value(0).toString());
        xml->createElement( "offer[" + QString::number(i) + "].name" ,query.value(1).toString());
        xml->createElement( "offer[" + QString::number(i) + "].count" ,query.value(2).toString());
        i++;
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}

bool HistoricDB::getXOptionSection(XmlConfig *xml)
{


    bool ret = false;

    if (!xml) return false;

    xml->pushDomain();
    xml->delDomain();

    if (!isConnected()){
        xml->popDomain();
        return false;
    }

    QString sql = "SELECT option_type, prod_option, num_products, total FROM z_options order by option_type; ";
    QSqlQuery query(sql, getDB());
    if (!query.isActive()){
        cout << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        xml->popDomain();

        return false;
    }

    if (!query.size()){
        cout << "problems at" << __PRETTY_FUNCTION__ <<":"<< __LINE__ << endl;
        xml->createElement("options");
        return true;
    }

    int i = 0;
    xml->createElementSetDomain("options");
    while (query.next()){
        xml->createElement( "option[" + QString::number(i) + "].type" ,query.value(0).toString());
        xml->createElement( "option[" + QString::number(i) + "].name" ,query.value(1).toString());
        xml->createElement( "option[" + QString::number(i) + "].count" , query.value(2).toString());
        xml->createElement( "option[" + QString::number(i) + "].amount" , query.value(3).toString());
        i++;
    }
    xml->delDomain();
    xml->popDomain();
    return true;
}
