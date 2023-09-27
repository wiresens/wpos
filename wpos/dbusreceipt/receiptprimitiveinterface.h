/***************************************************************************
                          dbusreceiptprimitiveinterface.h  -  description
                             -------------------
    begin                : Thu Jan 8 2004
    copyright            : (C) 2004 by Napsis S.L.
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
#ifndef RECEIPTPRIMITIVEINTERFACE_H
#define RECEIPTPRIMITIVEINTERFACE_H

#include <QString>
/**
  *@author Carlos Manzanedo
  */

class ReceiptPrimitiveInterface  {
    virtual bool createReceipt(QString)=0;
    virtual bool saveReceipt(QString)=0;

    virtual bool deleteReceiptByStartDate(QString, QString)=0;

    virtual QString getReceiptByStartDate(QString, QString)=0;
    virtual QString getReceiptResume()=0;
    virtual QString getReceiptResume(QString)=0;

    virtual bool lockReceiptByStartDate(QString, QString)=0;
    virtual bool unlockReceiptByStartDate(QString, QString)=0;

    virtual bool existsReceiptByStartDate(QString, QString)=0;
    virtual bool getReceiptStateByStartDate(QString, QString)=0;

};

#endif




