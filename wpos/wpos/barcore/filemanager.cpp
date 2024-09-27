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

FileManager::FileManager(QObject *parent, const QString& name ) :
    QObject(parent),
    db {new FileManagerDB("FileManagerConnection", Files::configFilePath("database"))}
{
    setObjectName(name);
}

FileManager::~FileManager(){
    files.clear();
}

void FileManager::addFile(const QString& file){
    files.append(file);
}

void FileManager::removeFile(const QString& file){
    files.removeAll(file);
}

void FileManager::saveFiles(){

    db->connect();
    for (auto& file : files){
        db->delXmlFile(file);
        db->saveXmlFile(file);
    }
    db->disConnect();
}

void FileManager::loadFiles(){

    db->connect();
    for (auto& file : files)
        db->loadXmlFile(file);
    db->disConnect();
}






