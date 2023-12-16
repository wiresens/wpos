/***************************************************************************
                          filemanagerdb.h  -  description
                             -------------------
    begin                : mi� jun 2 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef FILEMANAGERDB_H
#define FILEMANAGERDB_H

#include <basicdatabase.h>
#include <wposcore/hlist.h>

class XmlConfig;

class FileManagerDB : public BasicDatabase  {
public:
    FileManagerDB(
        const QString& connection,
        const QString& hostname,
        const QString& database,
        const QString& username,
        const QString& passwd);

    FileManagerDB(
        const QString& connection,
        XmlConfig *xml);

    FileManagerDB(
        const QString& connection,
        const QString& configFile);

    bool exists(const QString& file);
    bool loadXmlFile(const QString& file);
    bool saveXmlFile(const QString& file);
    bool delXmlFile(const QString& file);
};

#endif
