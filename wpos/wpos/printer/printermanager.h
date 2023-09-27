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

class PrinterManager :public QObject{
public:
    explicit PrinterManager(QObject *parent=nullptr, const QString& name = QString());
    ~PrinterManager();

    bool printTicket(XmlConfig* xml, int times=1 );
    bool printInvoice(XmlConfig* xml, int times=2 );
    bool printX(XmlConfig* xml);
    bool printZ(XmlConfig* xml);
    bool printTicketTotal(XmlConfig* xml);
    bool printKitchenOrder(XmlConfig* xml);
};

#endif
