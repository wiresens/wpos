/***************************************************************************
                          bslkillticketsdb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef KILLTICKETSDB_H
#define KILLTICKETSDB_H

#include <basicdatabase.h>
#include <QtCore/QList>
/**
  *@author BSTeam
  */

struct TicketResumeData{
    QString ticket_code;
    QString employee_id;
    QString employee_name;
    QString timestamp;
    QString total;
    QString ticket_state;
    QString table_code;
    bool receipt_state;
};

using TicketResumes = QVector<TicketResumeData>;

class KillTicketsDB : public BasicDatabase {
public:  
    KillTicketsDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    KillTicketsDB(
        const QString& connection,
        const QString& configFile);

    /**
    * invoices sequences. (Invoices are attached to tickets).
    */
    int getNextInvoiceVal();
    bool setNextInvoiceVal(int val);

    /**
    *       get a list with a resume of all tickets
    */
    TicketResumes getTicketResume();
    TicketResumes getReceiptResume(QString employee_id);
    TicketResumes getReceiptResume();

    /**
    *       @param a qstring with the ticket_code in a QString or a int with the ticket
    *       @return true if the ticket has any price with value < 0.
    */
    bool ticketHasNegative(const QString& ticket);
    bool ticketHasNegative(int ticket);

    /**
    *       invoice methods to obtain and insert invoices.
    */
    int getInvoiceFromTicket(int ticket);
    bool groupInvoiceWithTicket(int ticket_code, int invoice_code);


    /**
    * get a ticket from the DataBase. This method return a Xml with the composition
    * of the ticket at the database. The method will be moved in the future to the TicketCoreDB
    * class if any other DB method need the Xml from the Database.
    */
//    XmlConfig *getTicketFromDatabase(int ticket_number);
    XmlConfig getTicketFromDatabase(int ticket_number);
};

#endif
