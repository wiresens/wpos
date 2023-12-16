/***************************************************************************
                          bslchangemoneywidgetdb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "changemoneywidgetdb.h"

ChangeMoneyWidgetDB::ChangeMoneyWidgetDB(
    const QString& connection,
    const QString& hostname,
    const QString& database,
    const QString& username,
    const QString& passwd):
    BasicDatabase(connection,hostname, database, username,passwd) {
}

ChangeMoneyWidgetDB::ChangeMoneyWidgetDB(
    const QString& connection,
    const QString& configFile):
    BasicDatabase(connection,configFile){
}

double ChangeMoneyWidgetDB::getMoneyInCash () {
    double result = -1;
    if (isConnected()){
        QString sql = "SELECT sum(quantity) FROM cash_movements;";
        QSqlQuery query (sql, getDB());

        if (query.isActive()){
           query.first();
           result = query.value(0).toDouble();
        }
    }
    return result;
}

double ChangeMoneyWidgetDB::getMoneyPayType(const QString& type){
    double ret = 0.00;
    if (isConnected()){
        QString sql = "SELECT sum(c.quantity) FROM ";
        sql+= "tickets t JOIN cash_movements c ";
        sql+= "USING (ticket_code) ";
        sql+= "WHERE t.pay_type='"+type+"' ;";
        QSqlQuery query( sql, getDB());

        if (query.isActive()){
           query.first ();
           ret = query.value(0).toDouble();
        }
    }
    return ret;
}



