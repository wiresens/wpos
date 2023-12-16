/***************************************************************************
                          printermanager.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/


#ifndef _PRINTERMANAGER_H
#define _PRINTERMANAGER_H

#include <QObject>

class XmlConfig;

class PrinterManager : public QObject{
Q_OBJECT

public:
    explicit PrinterManager(
        QObject *parent=nullptr,
        const QString& name = QString());

    bool printTicket(const XmlConfig& xml, int times=1 );
    bool printInvoice(const XmlConfig& xml, int times=2 );
    bool printX(const XmlConfig& xml);
    bool printZ(const XmlConfig& xml);
    bool printTicketTotal(const XmlConfig& xml);
    bool printKitchenOrder(const XmlConfig& xml);
};

#endif
