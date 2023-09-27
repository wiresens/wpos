/***************************************************************************
                          dcopreceiptcomm.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "dbusreceiptquerier.h"
#include "dbusreceiptprimitive_interface.h"
#include "dbusreceiptmediator_interface.h"

#include <xmlconfig.h>
#include <QDataStream>

using namespace std;
using IReceiptPrimitive = com::wiresens::wpos::dbusreceipt::DBusReceiptPrimitive;
using IReceiptMediator = com::wiresens::wpos::dbusreceipt::DBusReceiptMediator;
static const QString dBusService = "com.wiresens.wpos.dbusreceipt";
static const QString dBusPrimitiveObject = "/wpos/dbusreceipt/DBusReceiptPrimitive";
static const QString dBusMediatorObject = "/wpos/dbusreceipt/DBusReceiptMediator";

DBusReceiptQuerier::DBusReceiptQuerier(QObject *parent):
    QObject(parent){}

//Using IReceiptPrimitive : ReadOnly Access
bool DBusReceiptQuerier::receiptExist(const QString &  employee_id, const QString & start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptPrimitive(dBusService, dBusPrimitiveObject, bus, this);
    auto method = "DBusReceiptQuerier::receiptExist(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusPrimitiveObject, method );
        return false;
    }

    auto reply = iface->existsReceiptByStartDate(employee_id, start_time);
    if( reply.isError() ) {
        log(dBusService, dBusPrimitiveObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

bool DBusReceiptQuerier::receiptState(const QString& employee_id, const QString& start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptPrimitive(dBusService, dBusPrimitiveObject, bus, this);
    auto method = "DBusReceiptQuerier::receiptState(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusPrimitiveObject, method);
        return false;
    }

    auto reply = iface->getReceiptStateByStartDate(employee_id, start_time);
    if( reply.isError() ) {
        log(dBusService, dBusPrimitiveObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

XmlConfig* DBusReceiptQuerier::getReceipt(const QString& employee_id, const QString &start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptPrimitive(dBusService, dBusPrimitiveObject, bus, this);
    auto method = "DBusReceiptQuerier::getReceipt(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusPrimitiveObject, method);
        return nullptr;
    }
    auto reply =  iface->getReceiptByStartDate(employee_id, start_time);
    if( reply.isError()){
        log(dBusService, dBusPrimitiveObject, method, reply.error().message() );
        return nullptr;
    }

    auto result = new XmlConfig();
    result->readXmlFromString(reply.value());
    return result;
}

XmlConfig* DBusReceiptQuerier::getReceiptResume(){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptPrimitive(dBusService, dBusPrimitiveObject, bus, this);
    auto method = "DBusReceiptQuerier::getReceiptResume()";
    if (!iface->isValid()){
        log(dBusService, dBusPrimitiveObject, method);
        return nullptr;
    }

    auto reply =  iface->getReceiptResume();
    if( reply.isError() ) {
        log(dBusService, dBusPrimitiveObject, method, reply.error().message() );
        return nullptr;
    }

    auto result = new XmlConfig();
    result->readXmlFromString(reply.value());
    return result;
}

XmlConfig* DBusReceiptQuerier::getReceiptResume(QString employee_id){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptPrimitive(dBusService, dBusPrimitiveObject, bus, this);
    auto method = "DBusReceiptQuerier::getReceiptResume()";
    if ( !iface->isValid() ){
        log(dBusService, dBusPrimitiveObject, method);
        return nullptr;
    }

    auto reply =  iface->getReceiptResume(employee_id);
    if( reply.isError() ) {
        log(dBusService, dBusPrimitiveObject, method, reply.error().message() );
        return nullptr;
    }

    auto result = new XmlConfig();
    result->readXmlFromString(reply.value());
    return result;
}

//Using IReceiptMediator : ReadWrite Access
bool DBusReceiptQuerier::createReceipt(XmlConfig *xml){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptMediator(dBusService, dBusMediatorObject, bus, this);
    auto method = "DBusReceiptQuerier::createReceipt()";
    if (!iface->isValid()){
        log(dBusService, dBusMediatorObject, method);
        return false;
    }

    auto reply = iface->createReceipt(xml->toString());
    if( reply.isError() ){
        log(dBusService, dBusMediatorObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

bool DBusReceiptQuerier::saveReceipt(XmlConfig *xml){

    auto bus = QDBusConnection::sessionBus();
    IReceiptMediator *iface;
    iface = new IReceiptMediator(dBusService, dBusMediatorObject, bus, this);
    auto method = "DBusReceiptQuerier::saveReceipt()";
    if (!iface->isValid()){
        log(dBusService, dBusMediatorObject, method);
        return false;
    }
    auto reply = iface->saveReceipt(xml->toString());
    if( reply.isError()) {
        log(dBusService, dBusMediatorObject, method, reply.error().message() );
        return false;
    }

    return reply.value();
}

bool DBusReceiptQuerier::deleteReceipt(const QString employee_id, const QString start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptMediator(dBusService, dBusMediatorObject, bus, this);
    auto method = "DBusReceiptQuerier::deleteReceipt(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusMediatorObject, method);
        return false;
    }
    auto reply = iface->deleteReceiptByStartDate(employee_id, start_time);
    if( reply.isError()){
        log(dBusService, dBusMediatorObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

bool DBusReceiptQuerier::lockRemoteReceipts(const QString &employee_id, const QString& start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptMediator(dBusService, dBusMediatorObject, bus, this);
    auto method = "DBusReceiptQuerier::lockRemoteReceipts(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusMediatorObject, method);
        return false;
    }
    auto reply = iface->lockRemoteByStartDate(employee_id, start_time);
    if( reply.isError()) {
        log(dBusService, dBusMediatorObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

bool DBusReceiptQuerier::unlockRemoteReceipts(const QString &employee_id, const QString& start_time){

    auto bus = QDBusConnection::sessionBus();
    auto iface = new IReceiptMediator(dBusService, dBusMediatorObject, bus, this);
    auto method = "DBusReceiptQuerier::unlockRemoteReceipts(" + employee_id + "," + start_time + ")";
    if (!iface->isValid()){
        log(dBusService, dBusMediatorObject, method);
        return false;
    }
    auto reply = iface->unlockRemoteByStartDate(employee_id, start_time);
    if( reply.isError()) {
        log(dBusService, dBusMediatorObject, method, reply.error().message() );
        return false;
    }
    return reply.value();
}

void DBusReceiptQuerier::log(const QString& svc, const QString& obj, const QString& method, const QString& msg ){
    qDebug() << "DBus Service: " << svc << " for object: " << obj
             << "Requested by " << method;
    if( msg.isNull() || msg.isEmpty())
        qDebug() << "Not Available";
    else
        qDebug() << "Returned Error : " << msg;
}
