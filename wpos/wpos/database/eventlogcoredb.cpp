/***************************************************************************
                          eventlogcoredb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "eventlogcoredb.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVariant>


#include <iostream>
using namespace std;

EventLogCoreDB::EventLogCoreDB(const QString& _connection_name,
                               const QString& _hostname,
                               const QString& _database,
                               const QString& _username,
                               const QString& _passwd):
    BasicDatabase(_connection_name,_hostname, _database, _username,_passwd){}

EventLogCoreDB::EventLogCoreDB(const QString& _connection_name,
                               const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){}

EventLogCoreDB::~EventLogCoreDB(){}

int EventLogCoreDB::getNextItemVal(){
    int ret{ -1 };
    if (!isConnected()) return ret;

    QString query  {"SELECT nextval('event_log_event_code_seq');"};
    QSqlQuery query_obj {QSqlQuery(query, getDB())};

    if (!query_obj.isActive() || !query_obj.size() ) return ret;

    query_obj.first();
    return query_obj.value(0).toInt();
}

void EventLogCoreDB::logData(const EventLogData *data){

    if (! isConnected())  return;

    /* Write to the event_log */
    QString query {"INSERT INTO event_log (event_code, employee_id, time_stamp, event_type) VALUES ("};
    query += data->event_code+",";
    query += "'"+data->employee_id+"',";
    query += "'"+data->timestamp+"',";
    query += "'"+data->event_type+"'";
    query += ");";

    QSqlQuery query_obj {QSqlQuery(query, getDB())};
    if (!query_obj.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }

    //the quantity. This is valid for tickets and cashbox movements
    bool ok;
    data->quantity.toDouble(&ok);
    if (data->quantity.isEmpty() || !ok)  return;

    query = "INSERT INTO cash_movements (event_code, quantity) VALUES (";
    query += data->event_code+", ";
    query += data->quantity+");";

    query_obj =  QSqlQuery(query, getDB());
    if (!query_obj.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }

    data->ticket_number.toInt(&ok);
    if (data->ticket_number.isEmpty() || !ok)  return;

    // If ticket event then stamp the ticket_code
    query = "UPDATE cash_movements ";
    query += "SET ticket_code="+data->ticket_number+" ";
    query += "WHERE event_code="+data->event_code+";";

    query_obj = QSqlQuery(query,getDB());
    if (!query_obj.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }
}
