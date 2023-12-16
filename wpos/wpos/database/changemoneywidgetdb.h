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
    ChangeMoneyWidgetDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    ChangeMoneyWidgetDB(
        const QString& connection,
        const QString& configFile);

    double getMoneyInCash();
    double getMoneyPayType(const QString& type);
};

#endif
