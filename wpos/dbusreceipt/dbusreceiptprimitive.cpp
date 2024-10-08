/***************************************************************************
                          dbusreceiptprimitive.cpp  -  description
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

#include "dbusreceiptprimitive.h"
#include "dbusreceiptprimitive_adaptor.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>

#include <QFileSystemWatcher>
#include <QCoreApplication>
#include <QString>

const QString DBusReceiptPrimitive::DBusObject  = QString{"/wpos/dbusreceipt/DBusReceiptPrimitive"};

DBusReceiptPrimitive::DBusReceiptPrimitive(
    QObject *parent,
    const QString& name
):
    QObject(parent),
    CON_NAME{name},
    CON_CFG_FILE{cfg::xmlFileByKey(cfg::XMLKey::Database)},
    receipt_db {new ReceiptDB(CON_NAME, CON_CFG_FILE)}
{
    new DBusReceiptPrimitiveAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DBusReceiptPrimitive::DBusObject, this);

    setObjectName(name);

    //it should be used by other modules so please be sure that if not implemented here it
    //should be created before all the modules are created.
    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(CON_CFG_FILE);

    connect(file_watcher, &QFileSystemWatcher::fileChanged,
            this, &DBusReceiptPrimitive::fileDirtySlot);
}

DBusReceiptPrimitive::~DBusReceiptPrimitive(){
    delete receipt_db;
}

bool DBusReceiptPrimitive::createReceipt(QString xml_receipt){

    XmlConfig xml;
    if (xml_receipt.isEmpty() || !xml.readXmlFromString(xml_receipt))
        return false;

    bool ret = receipt_db->create(xml, false);
    if (ret){
        xml.delDomain();
        auto employee_id = xml.readString("employee.dni");
        auto start_time = xml.readString("timestamps.start");
        emit receiptChangedByStartDate(employee_id, start_time);
    }
    return ret;
}

bool DBusReceiptPrimitive::saveReceipt(QString xml_receipt){

    XmlConfig xml;
    if (xml_receipt.isEmpty()|| !xml.readXmlFromString(xml_receipt))
        return false;

    bool saved = receipt_db->saveReceipt(xml);
    if (saved){
        xml.delDomain();
        auto employee_id = xml.readString("employee.dni");
        auto start_time = xml.readString("timestamps.start");
        emit receiptChangedByStartDate(employee_id, start_time);
    }
    return saved;
}

bool DBusReceiptPrimitive::deleteReceiptByStartDate(QString employee_id, QString start_time){
    bool deleted = false;
    deleted = receipt_db->deleteReceiptByDate(employee_id,start_time);
    if (deleted)
        emit receiptChangedByStartDate(employee_id,start_time);

    return deleted;
}

QString DBusReceiptPrimitive::getReceiptByStartDate(QString employee_id, QString start_time){
    return receipt_db->getReceiptByDate(employee_id,start_time);
}

QString DBusReceiptPrimitive::getReceiptResume(){
    return receipt_db->getReceiptResume();
}

QString DBusReceiptPrimitive::getReceiptResume(QString employee_id){
    return receipt_db->getReceiptResume(employee_id);
}

bool DBusReceiptPrimitive::lockReceiptByStartDate(QString employee_id, QString start_time){
    bool locked = false;
    locked = receipt_db->lockReceiptByDate(employee_id, start_time);
    if (locked) emit receiptChangedByStartDate(employee_id, start_time);
    return locked;
}

bool DBusReceiptPrimitive::unlockReceiptByStartDate(QString employee_id, QString start_time){
    bool unlocked = false;
    unlocked = receipt_db->unlockReceiptByDate(employee_id,start_time);
    if (unlocked)
        emit receiptChangedByStartDate(employee_id,start_time);
    return unlocked;
}

bool DBusReceiptPrimitive::existsReceiptByStartDate(QString employee_id, QString start_date){
    qDebug() << "Begin DBus Service Request DBusReceiptPrimitive::existsReceiptByStartDate() ";

    bool exist = false;
    exist = receipt_db->existReceiptByDate(employee_id, start_date);
    if (exist)  emit receiptChangedByStartDate(employee_id, start_date);

    qDebug() << "End DBus Service Request DBusReceiptPrimitive::existsReceiptByStartDate()";
    qDebug() << "Result : " << exist;
    return exist;
}

bool DBusReceiptPrimitive::getReceiptStateByStartDate(QString employee_id, QString start_time){
    bool exists = false;
    exists = receipt_db->getReceiptStateByDate(employee_id, start_time);
    if (exists)
        emit receiptChangedByStartDate(employee_id,start_time);
    return exists;
}

void DBusReceiptPrimitive::fileDirtySlot(const QString& file){

    if (file == CON_CFG_FILE){
        qDebug() << qApp->applicationName() <<" : Rereading database configuration";
#ifndef _WINDOWS
        usleep(2000);
#endif
        XmlConfig xml (CON_CFG_FILE);
        if (xml.wellFormed()){
            delete receipt_db;
            receipt_db = new ReceiptDB(CON_NAME, CON_CFG_FILE);
        }
    }
}
