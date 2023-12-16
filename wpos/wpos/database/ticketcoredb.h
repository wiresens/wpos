/***************************************************************************
                          ticketcoredb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef TICKETCOREDB_H
#define TICKETCOREDB_H

#include <basicdatabase.h>

struct TicketData{
    int ticket_number;
    QString employee_id;
    QString start_time;
    QString end_time;
    QString pay_type;
    QString ticket_state;
};

struct TicketItemData{
    int item_code;
    int ticket_number;
    QString product_code;
    double price;
    int units;
    QString order_time;
    QString tax;
    double tax_rate;
};

struct TicketLocationData{
    int ticket_number;
    QString lounge;
    QString table;
};

struct TicketItemOptionData{
    int item_code;
    QString option_type;
    QString option;
    QString value;
};

class XmlConfig;
class QString;
class OfferData;

class TicketCoreDB : public BasicDatabase  {
public:
    TicketCoreDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    TicketCoreDB(
        const QString& connection,
        const QString& configFile);

    int getNextVal();
    bool setVal(int val);

    int getNextItemVal();
    bool setNextItemVal(int val);

    bool insertTicket(TicketData* data);
    bool insertLocation(TicketLocationData* data);
    bool insertTicketItem(TicketItemData* data);
    bool insertItemOption(TicketItemOptionData* data);
    bool insertOffer(OfferData* data);

    bool getXEmployeeData(XmlConfig* xml);
    bool getXMain(XmlConfig* xml);
    bool getXOfferSection(XmlConfig* xml);
    bool getXOptionSection(XmlConfig* xml);

    int getEmployeeProductInvitations(const QString& employee_id, XmlConfig* xml);
    int getEmployeeProductOptions(const QString& employee_id, XmlConfig* xml);
    int getEmployeeEvent(const QString& employee_id, const QString& event_type);

    //     bool getXStatistics(XmlConfig* xml);
    bool getXProducts(XmlConfig* xml, QStringList product_code_list);
    bool getXProductZeroPriced(XmlConfig* xml);

    int getFirstTicketCode();
    QString getFirstTicketDate();
    QString getLastZDate();

    int getLastTicketCode();
    QString getLastTicketDate();

    bool ticketCodeExists(int ticket_code);
    QString dateFromTicketCode(int ticket_code);

    bool vaccum();
};

#endif
