/***************************************************************************
                          linuxbarcomunicator.h  -  description
                             -------------------
    begin                : Tue Jan 13 2004
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

#ifndef RECEIPTCHANGENOTIFIYER_H
#define RECEIPTCHANGENOTIFIYER_H

#include <QObject>

/**
  *@author Carlos Manzanedo
  */

class ReceiptChangeNotifiyer : public QObject  {
    Q_OBJECT
public:
    explicit ReceiptChangeNotifiyer(QObject *parent = nullptr, const QString& name = QString());

protected slots:
    void receiptChangedSlotByDate(const QString& employee_id, const QString& start_time);
};

#endif
