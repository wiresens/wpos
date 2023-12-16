/***************************************************************************
                          eventlogcoredb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef EVENTLOGCOREDB_H
#define EVENTLOGCOREDB_H

#include <basicdatabase.h>

class QString;
class XmlConfig;

struct EventLogData{
    QString event_code;
    QString employee_id;
    QString timestamp;
    QString event_type;
    QString quantity;
    QString ticket_number;
};

class EventLogCoreDB : public BasicDatabase  {
public:
    EventLogCoreDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    EventLogCoreDB(
        const QString& connection,
        const QString& configFile);

    int getNextItemVal();
    void logData(const EventLogData &data);
};

#endif
