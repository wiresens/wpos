/***************************************************************************
                          filemanager.cpp  -  description
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

#include "filemanager.h"

#include "database/filemanagerdb.h"
#include <wposcore/config.h>
#include <QCoreApplication>
#include <libbslxml/xmlconfig.h>

FileManager::FileManager(QObject *parent, const QString& name ) :
    QObject(parent),
    m_dataSource { std::move(FileManagerDB("FileManagerConnection", cfg::xmlFileByKey(cfg::XMLKey::Database)))}
{
    setObjectName(name);
}

FileManager::~FileManager(){
    // delete dataStore;
}

void FileManager::addFile(const QString& file){
    m_files.append(file);
}

void FileManager::registerFiles(const QStringList& files){
    m_files = files;
}

void FileManager::removeFile(const QString& file){
    m_files.removeAll(file);
}

void FileManager::saveFiles(){
    m_dataSource.connect();
    m_dataSource.startTransaction();
        for (auto& file : m_files){
            m_dataSource.delXmlFile(file);
            m_dataSource.saveXmlFile(file);
        }
    m_dataSource.commit();
    m_dataSource.disConnect();
}

// void FileManager::fetchConfigFiles(
//     const QStringList& files)
// {
//     m_dataSource.connect();
//         auto res = m_dataSource.loadXmlFiles(files);
//         saveFilesLocaly(res, cfg::CFG_XML_DIR);
//     m_dataSource.disConnect();
// }

void FileManager::fetchConfigFiles(
    const QStringList& files)
{
    m_dataSource.connect();
    auto res = m_dataSource.loadXmlFiles(files);
    saveFilesLocaly(res);
    m_dataSource.disConnect();
}

// void FileManager::saveFilesLocaly(
//     const QVector<FileRecord>& records,
//     const QString& dir)
// {
//     for( auto &rec : records){
//         XmlConfig xml;
//         xml.readXmlFromString(rec.m_file_contents);
//         if (xml.wellFormed()){
//             auto abs_file_path = dir + rec.m_file_name;
//             if (xml.save(abs_file_path))
//                 qDebug() << "Saved " << rec.m_file_name
//                          << " to disk at " << xml.fileName();
//         }
//     }
// }

void FileManager::saveFilesLocaly(
    const QVector<FileRecord>& records)
{
    for( auto &rec : records){
        XmlConfig xml;
        xml.readXmlFromString(rec.m_file_contents);
        if (xml.wellFormed()){
            auto abs_file_path = rec.m_file_path + rec.m_file_name;
            xml.save(abs_file_path);
        }
    }
}
