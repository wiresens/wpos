/***************************************************************************
                          dcopprinterinterface.h  -  description
                             -------------------
    begin                : lun may 10 2004
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

#ifndef H_DBUS_PRINTER_INTERFACE_H
#define H_DBUS_PRINTER_INTERFACE_H

#include <QString>

class DBusPrinterInterface {
    virtual void printTicket(QString,int)=0;
    virtual void printInvoice(QString,int)=0;
    virtual void printX(QString)=0;
    virtual void printZ(QString)=0;
    virtual void printTicketTotal(QString)=0;
    virtual void printKitchenOrder(QString)=0;

    virtual bool realPrintTicket(QString,int)=0;
    virtual bool realPrintInvoice(QString,int)=0;
    virtual bool realPrintX(QString)=0;
    virtual bool realPrintZ(QString)=0;
    virtual bool realPrintTicketTotal(QString)=0;
};

#endif
