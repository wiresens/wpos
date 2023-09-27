/***************************************************************************
                          ticketcore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef TICKETCORE_H
#define TICKETCORE_H

#include <QObject>

class XmlConfig;
class QString;
class TicketCoreDB;
class PrinterManager;

class TicketCore : public QObject  {
    Q_OBJECT
public:
    explicit TicketCore(QObject *parent = nullptr,  const QString& name = QString());
    ~TicketCore();

    bool getTogglePrinterState();

public slots:
    void receiveCoreDataSlot(XmlConfig *_xml);
    void togglePrinterState(bool on);
    void xSlot();
    void zSlot();

    void setTicketCopies(uint copies);
    void genericSignalSlot(const QString& signal);
    void genericDataSignalSlot(const QString& signal, XmlConfig *xml);

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected:
    bool dailyCashOperations(bool partial = true);
    bool stampTicket(XmlConfig *_xml);
    QString getProductCode(XmlConfig *_xml);
    void fillTicketGaps();

    TicketCoreDB *db{};
    PrinterManager *printer{};
    int num_copy{1};
    bool printer_state{false};

};

#endif
