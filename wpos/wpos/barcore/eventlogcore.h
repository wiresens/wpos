/***************************************************************************
                          eventlogcore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef EVENTLOGCORE_H
#define EVENTLOGCORE_H

#include <QObject>


class QString;
class XmlConfig;
class EventLogCoreDB;

class EventLogCore : public QObject{
    Q_OBJECT
public:
    explicit EventLogCore(QObject *parent = 0, const QString& name = QString());
    ~EventLogCore();

public slots:
    void genericDataSignalSlot(const QString& signal_name,XmlConfig *_xml);

protected:
    void processEventLog(XmlConfig *xml);
    EventLogCoreDB *db;
};

#endif
