/***************************************************************************
                          historicdb.h  -  description
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

#ifndef HISTORICDB_H
#define HISTORICDB_H

#include <wposcore/basicdatabase.h>
#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QList>

struct TicketResumeData{
    QString ticket_code;
    QString employee_id;
    QString employee_name;
    QString stamp_time;
    QString total;
    QString ticket_state;
    QString table_code;
    bool receipt_state;
};

struct ZResumeData{
    QString start_date;
    QString end_date;
    QString cash;
    QString event_code;
};

class HistoricDB :
        public BasicDatabase {
public:
    HistoricDB(const QString& _connection_name,
               const QString& _hostname,
               const QString& _database,
               const QString& _username,
               const QString& _passwd);

    HistoricDB(const QString& _connection_name,
               const QString& configuration_path);

    ~HistoricDB();

    /**
        *       get a list with a resume of all tickets
        */
    QVector<TicketResumeData> getTicketResume(QString start_date, QString end_date);
    QList<ZResumeData*>* getZResume(QString start_date,QString end_date);

    /**
        * get a ticket from the DataBase. This method return a Xml with the composition
        * of the ticket at the database. The method will be moved in the future to the TicketCoreDB
        * class if any other DB method need the Xml from the Database.
        */
    XmlConfig *getTicketFromDatabase (int ticket_number);

    bool getXEmployeeData(XmlConfig *xml);
    bool getXMain(XmlConfig *xml);
    bool getXOfferSection(XmlConfig *xml);
    bool getXOptionSection(XmlConfig *xml);

    int getEmployeeProductInvitations(const QString& employee_id, XmlConfig *xml);
    int getEmployeeProductOptions(const QString& employee_id, XmlConfig *xml);
    int getEmployeeEvent(const QString& employee_id, const QString& event_type);

    double getMoneyInCash();
    bool createTempZ(const QString& code);
};

#endif
