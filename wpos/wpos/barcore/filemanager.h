/***************************************************************************
                          filemanager.h  -  description
                             -------------------
    begin                : jue jun 3 2004
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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QStringList>
#include "database/filemanagerdb.h"

class FileManager : public QObject  {
    Q_OBJECT
public:
    explicit FileManager(
        QObject *parent=nullptr,
        const QString& name = QString{});

    ~FileManager();

    void addFile(const QString& file);
    void registerFiles(const QStringList& files);
    void removeFile(const QString& file);
    void saveFiles();
    void fetchConfigFiles(const QStringList& files);

private:
    // void saveFilesLocaly(const QVector<FileRecord>& records, const QString &dir);
    void saveFilesLocaly(const QVector<FileRecord>& records);

private:
    QStringList m_files;
    FileManagerDB m_dataSource;
};

#endif
