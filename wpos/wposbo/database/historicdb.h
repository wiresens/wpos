// file      :  historicdb.h
// birth     :  5/11/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef HISTORICDB_H
#define HISTORICDB_H

#include <libbslxml/xmlconfig.h>
#include <wposcore/basicdatabase.h>

#include <QList>
#include <QString>

struct TicketResumeData {
    QString ticket_code;
    QString employee_id;
    QString employee_name;
    QString stamp_time;
    QString total;
    QString ticket_state;
    QString table_code;
    bool receipt_state;
};

struct ZResumeData {
    QString start_date;
    QString end_date;
    QString cash;
    QString event_code;
};

class HistoricDB : public BasicDatabase {
public:
    HistoricDB(const QString& connection_name,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    HistoricDB(const QString& _connection_name,
        const QString& configuration_path);

    /**
     *       get a list with a resume of all tickets
     */
    QVector<TicketResumeData> getTicketResume(QString start_date, QString end_date);
    QList<ZResumeData*>* getZResume(QString start_date, QString end_date);

    /**
     * get a ticket from the DataBase. This method return a Xml with the composition
     * of the ticket at the database. The method will be moved in the future to the TicketCoreDB
     * class if any other DB method need the Xml from the Database.
     */
    XmlConfig getTicketFromDatabase(int ticket_number);

    bool getXEmployeeData(XmlConfig* xml);
    bool getXMain(XmlConfig* xml);
    bool getXOfferSection(XmlConfig* xml);
    bool getXOptionSection(XmlConfig* xml);

    int getEmployeeProductInvitations(const QString& employee_id, XmlConfig* xml);
    int getEmployeeProductOptions(const QString& employee_id, XmlConfig* xml);
    int getEmployeeEvent(const QString& employee_id, const QString& event_type);

    double getMoneyInCash();
    bool createTempZ(const QString& code);
};

#endif
