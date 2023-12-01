/***************************************************************************
                          auxdb.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef AUXDB_H
#define AUXDB_H

#include <basicdatabase.h>
#include <QList>
#include <wposcore/hlist.h>
class ProductExtraInfo;
class XmlConfig;

class AuxDB : public BasicDatabase  {
public:

        AuxDB(const QString& _connection_name,
              const QString& _hostname,
              const QString& _database,
              const QString& _username,
              const QString& _passwd);

        AuxDB(const QString& _connection_name,
              XmlConfig *xml);

        AuxDB(const QString& _connection_name,
              const QString& configuration_path);

        ~AuxDB();

        HList<ProductExtraInfo>* getOptionNodes();
};

#endif
