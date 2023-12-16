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

    AuxDB(const QString& connection,
          const QString& hostname,
          const QString& database,
          const QString& username,
          const QString& passwd);

    AuxDB(
        const QString& connection,
        XmlConfig *xml);

    AuxDB(
        const QString& connection,
        const QString& configFile);

    QMap<QString, ProductExtraInfo> productExtra();
};

#endif
