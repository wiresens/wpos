/***************************************************************************
                          linubarreceiptinterface.h  -  description
                             -------------------
    begin                : Tue Jan 13 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef RECEIPTINTERFACE_H
#define RECEIPTINTERFACE_H

#include <QString>
/**
  *@author Carlos Manzanedo
  */

class ReceiptInterface {
    virtual void receiptChanged(QString, QString) = 0;
};

#endif
