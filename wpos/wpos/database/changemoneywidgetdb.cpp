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

ChangeMoneyWidgetDB::ChangeMoneyWidgetDB(const QString& _connection_name,
                       const QString& _hostname,const QString& _database,
                       const QString& _username,const QString& _passwd):
                       BasicDatabase(_connection_name,_hostname, _database, _username,_passwd) {
}

ChangeMoneyWidgetDB::ChangeMoneyWidgetDB(const QString& _connection_name,const QString& configuration_path):
                                   BasicDatabase(_connection_name,configuration_path){
}

ChangeMoneyWidgetDB::~ChangeMoneyWidgetDB(){
}

double ChangeMoneyWidgetDB::getMoneyInCash () {
QSqlQuery *q=0;
QString query;
double ret = -1;

     if (!this->isConnected())
          return ret;

     query = "SELECT sum(quantity) FROM cash_movements;";
     q = new QSqlQuery(query,this->getDB());

     if (!q->isActive()){
          delete q;
          return ret;
     }

     q->first ();

     ret = q->value (0).toDouble ();

     delete q;
     return ret;
}


double ChangeMoneyWidgetDB::getMoneyPayType(const QString& type){
QSqlQuery *q=0;
QString query;
double ret = 0.00;

     if (!this->isConnected())
          return ret;

     query = "SELECT sum(c.quantity) FROM ";
     query+= "tickets t JOIN cash_movements c ";
     query+= "USING (ticket_code) ";
     query+= "WHERE t.pay_type='"+type+"' ;";
     q = new QSqlQuery(query,this->getDB());

     if (!q->isActive()){
          delete q;
          return ret;
     }

     q->first ();

     ret = q->value (0).toDouble ();

     delete q;
     return ret;
}



