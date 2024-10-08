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

#include <wposcore/basicdatabase.h>
#include <wposcore/hlist.h>

class XmlConfig;

struct FileRecord{
    QString m_file_path;
    QString m_file_name;
    QString m_file_contents;
};

using ResultSet = QVector<FileRecord>;
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
    ResultSet loadXmlFiles(const QStringList &files);
    bool saveXmlFile(const QString& file_name);
    bool delXmlFile(const QString& file_name);
};

#endif
