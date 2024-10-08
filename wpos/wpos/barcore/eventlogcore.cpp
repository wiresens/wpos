/***************************************************************************
                          eventlogcore.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "eventlogcore.h"
#include "database/eventlogcoredb.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/utility.h>
#include <wposcore/config.h>
#include <xmlconfig.h>
#include "../auth/authcore.h"


#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDateTime>

#include <iostream>

using namespace std;

extern AuthCore *authCore;


EventLogCore::EventLogCore(QObject *parent,  const QString& name ):
    QObject(parent),
    db{new EventLogCoreDB("EventLogCoreConnection", cfg::xmlFileByKey(cfg::XMLKey::Database))}
{
    setObjectName(name);
    db->connect();

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::EVENTLOG,this);

}

EventLogCore::~EventLogCore(){
    db->disConnect();
    delete db;
}

void EventLogCore::genericDataSignalSlot(const QString& signal_name, XmlConfig *_xml){
    //event
    if (signal_name ==  GDATASIGNAL::EVENTLOG ){
        processEventLog(_xml);
    }
}

void EventLogCore::processEventLog(XmlConfig *xml){
    EventLogData data;

    xml->pushDomain();
    xml->delDomain();
    data.event_type = xml->readString("event_type");
    data.quantity = xml->readString("cash_quantity");
    data.ticket_number = xml->readString("ticket_number");

    if (data.event_type.isEmpty()){
        xml->popDomain();
        return;
    }

    if (authCore)
        data.employee_id = authCore->userId();

    data.event_code = QString::number(db->getNextItemVal());
    data.timestamp = currentDateTimeString();
    db->logData(data);
}
