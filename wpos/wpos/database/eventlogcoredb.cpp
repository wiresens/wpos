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

EventLogCoreDB::EventLogCoreDB(const QString& connection,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd):
    BasicDatabase(
        connection,hostname, database, username,passwd){}

EventLogCoreDB::EventLogCoreDB(const QString& connection,
    const QString& configFile):
    BasicDatabase(connection,configFile){}

int EventLogCoreDB::getNextItemVal(){
    int ret{ -1 };
    if (!isConnected()) return ret;

    QString sql  {"SELECT nextval('event_log_event_code_seq');"};
    QSqlQuery query {QSqlQuery(sql, dbHandle())};

    if (!query.isActive() || !query.size() ) return ret;

    query.first();
    return query.value(0).toInt();
}

void EventLogCoreDB::logData(const EventLogData &data){

    if (! isConnected())  return;

    /* Write to the event_log */
    QString sql {"INSERT INTO event_log (event_code, employee_id, time_stamp, event_type) VALUES ("};
    sql += data.event_code+",";
    sql += "'"+data.employee_id+"',";
    sql += "'"+data.timestamp+"',";
    sql += "'"+data.event_type+"'";
    sql += ");";

    QSqlQuery query {QSqlQuery(sql, dbHandle())};
    if (!query.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }

    //the quantity. This is valid for tickets and cashbox movements
    bool ok;
    data.quantity.toDouble(&ok);
    if (data.quantity.isEmpty() || !ok)  return;

    sql = "INSERT INTO cash_movements (event_code, quantity) VALUES (";
    sql += data.event_code+", ";
    sql += data.quantity+");";

    query =  QSqlQuery(sql, dbHandle());
    if (!query.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }

    data.ticket_number.toInt(&ok);
    if (data.ticket_number.isEmpty() || !ok)  return;

    // If ticket event then stamp the ticket_code
    sql = "UPDATE cash_movements ";
    sql += "SET ticket_code="+data.ticket_number+" ";
    sql += "WHERE event_code="+data.event_code+";";

    query = QSqlQuery(sql, dbHandle());
    if (!query.isActive()){
        cerr << "FAILURE IN " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return;
    }
}
