/***************************************************************************
                          bslchangemoneywidgetdb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef CHANGEMONEYWIDGETDB_H
#define CHANGEMONEYWIDGETDB_H

#include <basicdatabase.h>

class ChangeMoneyWidgetDB : public BasicDatabase  {
public: 
     ChangeMoneyWidgetDB(const QString& _connection_name,
                       const QString& _hostname,const QString& _database,
                       const QString& _username,const QString& _passwd);
     ChangeMoneyWidgetDB(const QString& _connection_name, const QString& configuration_path);
     ~ChangeMoneyWidgetDB();

     double getMoneyInCash();
     double getMoneyPayType(const QString& type);
};

#endif
