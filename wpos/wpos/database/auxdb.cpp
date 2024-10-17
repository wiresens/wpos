/***************************************************************************
                          auxdb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "auxdb.h"
#include <QVariant>
#include <libbslxml/xmlconfig.h>
#include <QString>

#include <wposcore/productextrainfo.h>

AuxDB::AuxDB(const QString& connection,
             const QString& hostname,
             const QString& database,
             const QString& username,
             const QString& passwd):
    BasicDatabase(
        connection,
        hostname,
        database,
        username,
        passwd)
{}

AuxDB::AuxDB(const QString& connection,
             XmlConfig *xml):
    BasicDatabase(connection,xml){}

AuxDB::AuxDB(const QString& connection,
             const QString& configFile):
    BasicDatabase(connection,configFile){}

QMap<QString, ProductExtraInfo> AuxDB::productExtra(){
    QMap<QString, ProductExtraInfo> extra;
    if ( isConnected() ){
        QString sql = "SELECT option_type, prod_option from products_options_list;";
        QSqlQuery query(sql, dbHandle());

        //prepare the query execution
        if ( query.isActive() && query.size()){
            while (query.next()){
                auto type = query.value(0).toString();
                auto value = query.value(1).toString();
                auto extraInfoIt = extra.find(type);
                if ( extraInfoIt == extra.end()){
                    ProductExtraInfo extraInfo(type);
                    extraInfo.addOption(value);
                    extra.insert(type, extraInfo);
                }
                else (*extraInfoIt).addOption(value);
            }
        }
    }
    return extra;
}
