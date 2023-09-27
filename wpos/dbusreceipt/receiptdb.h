/***************************************************************************
                          receiptdb.h  -  description
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

#ifndef RECEIPTDB_H
#define RECEIPTDB_H

#include <basicdatabase.h>
#include <wposcore/hlist.h>
#include <xmlconfig.h>
/**
  *@author Carlos Manzanedo
  */
 
class ReceiptDB : public BasicDatabase  {
public: 
        ReceiptDB(const QString& _connection_name,
                  const QString& _hostname,
                  const QString& _database,
                  const QString& _username,
                  const QString& _passwd);

        ReceiptDB(const QString& _connection_name, XmlConfig *xml);

        ReceiptDB(const QString& _connection_name,
                  const QString& configuration_path);

        ~ReceiptDB();

        bool create(XmlConfig& xml, bool lock=false);
        bool saveReceipt(XmlConfig& xml);

        bool existReceiptByDate(QString employee_id, QString start_date);
        bool lockReceiptByDate(QString employee_id, QString start_date);
        bool unlockReceiptByDate(QString employee_id, QString start_date);
        bool getReceiptStateByDate(QString employee_id, QString start_date);
        bool deleteReceiptByDate(QString employee_id, QString start_date);        

        QString getReceiptByDate(QString employee_id, QString start_date);
        QString getReceiptResume();
        QString getReceiptResume(QString employee_id);
};

#endif
