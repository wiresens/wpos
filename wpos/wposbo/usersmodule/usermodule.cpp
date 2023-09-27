/***************************************************************************
                          usermodule.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "usermodule.h"
#include "usermodule_adaptor.h"

#include "database/usersmoduledb.h"

#include <libbslxml/xmlconfig.h>

#include <QList>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <iostream>
using namespace std;

const QString& DCOP_OBJECT_INTERFACE_NAME = "UserModelInterface";

const QString& DCOPFX2000_FINGERPRINT_DIR="/etc/fx2000fingerprint/";
const QString& USER_DTD = "/etc/ntpv_backoffice/dtds/users_user.dtd";
const QString& USER_LIST_DTD = "/etc/ntpv_backoffice/dtds/users_userslist.dtd";
const QString& DCOPFX2000_ENROLL_DTD = "/etc/ntpv_backoffice/dtds/dcopfx2000_enroll.dtd";
const QString& DATABASE_CONNECTION_NAME = "UserModuleConnection";
const QString& DATABASE_DEFINITION_FILE = "/etc/ntpv/bar_database.xml";

const QString UserModule::DBusObjectPath  = QString{"/wpos/wposbo/DBusBOUser"};
UserModule::UserModule(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);
    new DBusBOUserAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(UserModule::DBusObjectPath, this);
}

bool UserModule::createUser(const QString& string_xml){

    UserData data {readUserFromXml(string_xml)};
    if ( data.isEmpty() ) return false;

    UsersModuleDB database(DATABASE_CONNECTION_NAME, DATABASE_DEFINITION_FILE);
    UserData aux_data { database.getUserInfo(data.employee_id) };

    if ( !aux_data.isEmpty() ){
        if ( database.getActiveFlag(data.employee_id) ){
            return false;
        }
        if (!database.updateUser(&data)){
            return false;
        }
        if (!database.setActiveFlag(data.employee_id,true)){
            return false;
        }
    }
    else{
        if (!database.addUser(&data)){
            return false;
        }
    }

    emit userCreated(data.employee_id);
    emit updatesAvailable();
    return true;
}

bool UserModule::changeUser(const QString& string_xml){

    UsersModuleDB database(DATABASE_CONNECTION_NAME, DATABASE_DEFINITION_FILE);
    UserData data {readUserFromXml(string_xml)};

    if ( data.isEmpty() || !database.updateUser(&data)) return false;
    emit userUpdated(data.employee_id);
    emit updatesAvailable();
    return true;
}

bool UserModule::deleteUser(const QString& employee_id){
    UsersModuleDB *db = 0;
    if (employee_id.isNull()||employee_id.isEmpty()||(employee_id == "1"))
        return false;

    db = new UsersModuleDB(DATABASE_CONNECTION_NAME,DATABASE_DEFINITION_FILE);
    if (!db->delUser(employee_id)){
        delete db;
        return false;
    }
    delete db;
    emit userDeleted(employee_id);
    emit updatesAvailable();
    return true;
}

bool UserModule::realDelUser(const QString& employee_id){
    UsersModuleDB *db = 0;
    if (employee_id.isNull()||employee_id.isEmpty()||(employee_id == "1"))
        return false;

    this->delUserFingerprints(employee_id);

    db = new UsersModuleDB(DATABASE_CONNECTION_NAME,DATABASE_DEFINITION_FILE);
    if (!db->realDelUser(employee_id)){
        delete db;
        return false;
    }
    delete db;

    emit userDeleted(employee_id);
    emit updatesAvailable();
    return true;
}

bool UserModule::delUserFingerprints(const QString& employee_id){

    if ( employee_id.isEmpty() || employee_id == "1")
        return false;

    QString path = DCOPFX2000_FINGERPRINT_DIR + employee_id + "*.wsp";
    QDir dir(path);
    bool status = true;
    if( ! dir.exists(path) ) return false;
    QStringList namefiltes {"*.wsp"};
        for (auto& file : dir.entryList(namefiltes, QDir::Readable| QDir::Writable | QDir::Files))
            status &= dir.remove(file);

        status &= dir.rmdir(path);
    return status;
}

QString UserModule::usersList(bool all){

    QVector<UserData> users{};
    UsersModuleDB database(DATABASE_CONNECTION_NAME, DATABASE_DEFINITION_FILE);
    users = database.getUserList(all);
    if(users.isEmpty()) return QString();

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("users");

    auto i = 0;
    for ( const auto& data : users){
        xml.createElementSetDomain( "user["+ QString::number(i++) + "]" );
        xml.createElement("employee_id", data.employee_id);
        xml.createElement("name", data.name);
        xml.createElement("last_name", data.last_name);
        xml.releaseDomain("user");
    }
    xml.delDomain();
    return xml.toString();
}

QString UserModule::usersExtendedList(bool all){

    UsersModuleDB database (DATABASE_CONNECTION_NAME, DATABASE_DEFINITION_FILE);
    QVector<UserData> users = database.getUserList(all);
    if(users.isEmpty()) return QString();

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("users");
    auto i = 0;
    for (const auto& data : users){
        xml.createElementSetDomain("user["+QString::number(i++)+"]");
        xml.createElement("employee_id",data.employee_id);
        xml.createElement("name",data.name);
        xml.createElement("last_name",data.last_name);
        xml.createElement("address",data.address);
        xml.createElement("nss",data.nss);
        xml.createElement("phone",data.phone);
        xml.createElement("cellular",data.cellular);
        xml.createElement("email",data.email);
        xml.createElement("company_id",data.company_id);
        xml.createElement("picture",data.picture);

        if (data.gerente) xml.createElement("gerente","true");
        else xml.createElement("gerente","false");
        xml.releaseDomain("user");
    }

    xml.delDomain();
    return xml.toString();
}

QString UserModule::getUserInfo(const QString& employee_id){

    UsersModuleDB database(DATABASE_CONNECTION_NAME,DATABASE_DEFINITION_FILE);
    UserData data{ database.getUserInfo(employee_id) };

    if ( data.isEmpty()) return QString();

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("user");
    xml.createElement("employee_id", data.employee_id);
    xml.createElement("name",data.name);
    xml.createElement("last_name",data.last_name);
    xml.createElement("address",data.address);
    xml.createElement("nss", data.nss);
    xml.createElement("phone",data.phone);
    xml.createElement("cellular",data.cellular);
    xml.createElement("email",data.email);
    xml.createElement("company_id",data.company_id);
    xml.createElement("picture",data.picture);

    if (data.gerente) xml.createElement("gerente","true");
    else  xml.createElement("gerente","false");

    xml.delDomain();
    return xml.toString();
}

UserData UserModule::readUserFromXml(const QString& string_xml){

    XmlConfig xml;
    if  ( !xml.readXmlFromString(string_xml) && !xml.validateXmlWithDTD(USER_DTD,true))
        return UserData();

    //read data from the xml
    xml.delDomain();
    xml.setDomain("user");
    UserData data;
    data.employee_id = xml.readString("employee_id");
    data.name = xml.readString("name");
    data.last_name = xml.readString("last_name");
    data.address = xml.readString("address");
    data.phone = xml.readString("phone");
    data.cellular = xml.readString("cellular");
    data.email = xml.readString("email");
    data.company_id = xml.readString("company_id");
    data.picture = xml.readString("picture");
    data.gerente = xml.readString("gerente") == "true";

    return data;
}

void UserModule::enrollUserData(const QString& _xml){
    XmlConfig *xml = 0;

    if (_xml.isEmpty())
        return;

    //validate the string to XML and the XML with a DTD
    xml = new XmlConfig();
    xml->readXmlFromString(_xml);
    if (!xml->isValid()){
        delete xml;
        return;
    }

    if (!xml->validateXmlWithDTD(DCOPFX2000_ENROLL_DTD,true)){
        delete xml;
        return;
    }
    delete xml;


    //if all is correct emit a string with all the data.
    emit enrollDataAvailable(_xml);
}

QString UserModule::getInterfaceName(){
    return DCOP_OBJECT_INTERFACE_NAME;
}

QString UserModule::getEnrollMethodName(){
    return  QString("enrollUserData(QString)");
}
