/***************************************************************************
                          databasemodule.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "databasemodule.h"
#include "databasemodule_adaptor.h"

#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QFile>
#include <QProcess>
#include <QtDBus/QDBusConnection>
#include <QFileSystemWatcher>

#include <iostream>
using namespace std;

static const QString DATABASE_CONFIG_DTD         {"dtddocs:dbmodule_config.dtd"};
static const QString DDBB_CONFIGURATION_DTD_NMAP {"dtddocs:dbmodule_config_nmap.dtd"};
static const QString DATABASE_DEFINITION_FILE    {"xmldocs:bar_database.xml"};

const QString DatabaseModule::DBusObject {"/wpos/wposbo/DBusBOModel"};

DatabaseModule::DatabaseModule(
    QObject *parent,
    const QString& name):
    QObject(parent)
{
    setObjectName(name);
    new DBusBOModelAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DatabaseModule::DBusObject, this);

    //it should be used by other modules so please be sure that if not implemented here it
    //should be created before all the modules are created.
    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(DATABASE_DEFINITION_FILE);
    connect(file_watcher, &QFileSystemWatcher::fileChanged,
        this, &DatabaseModule::handleFileChanged);
}

DatabaseModule::~DatabaseModule(){}

//the documentation for each method is described at the moduleinterface
bool DatabaseModule::setConfig(QString xml_str){

    //translate to a XmlConfig
    XmlConfig xml;
    xml.readXmlFromString(xml_str);
    if (!xml.wellFormed() || !xml.validateXmlWithDTD(DDBB_CONFIGURATION_DTD_NMAP))
        return false;

    //all's ok... first we are going to remove the config file and then we are going to save the xml at it
    QFile file(DATABASE_DEFINITION_FILE);
    if (file.exists()){
        if (!file.remove()){
            cerr << "problems while trying to delete the config file " << DATABASE_DEFINITION_FILE.toStdString() << endl;
            cerr << "at : "<<__PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
            return false;
        }
    }

    if (!xml.save(DATABASE_DEFINITION_FILE)){
        cerr << "problems while trying to save the config file " <<   DATABASE_DEFINITION_FILE.toStdString() << endl;
        cerr << "at : "<<__PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return false;
    }

    return true;
}

QString DatabaseModule::getConfig(){
    QString ret = "";
    XmlConfig *xml = 0;

    if (!(xml = this->loadDatabaseConfig()))
        return ret;

    ret = xml->toString();
    delete xml;
    return ret;
}

void DatabaseModule::stopDatabase(){
    QString host;
    QString command;
    QProcess process;
    XmlConfig *xml = 0;

    //module to be done... it should stop the database at the defined location.
    if (!(xml = loadDatabaseConfig())) return;
    xml->delDomain();
    host = xml->readString("database.hostname");
    delete xml;

    command += "ssh " + host +" /etc/init.d/postgres stop";
    process.setArguments( QStringList() << command);
    process.start();
}

void DatabaseModule::startDatabase(){
    QString host;
    QString command;
    QProcess process;
    XmlConfig *xml = 0;

    //module to be done... it should stop the database at the defined location.
    if (!(xml = this->loadDatabaseConfig()))
        return;
    xml->delDomain();
    host = xml->readString("database.hostname");
    delete xml;

    command += "ssh " + host +" /etc/init.d/postgres start";
    process.setArguments( QStringList() << command);
    process.start();
}

bool DatabaseModule::databaseState(){
    //check the state of the database...

    return true;
}

bool DatabaseModule::compact(){
    //make a vacum to the database.

    return true;
}

bool DatabaseModule::saveDump(){
    //makes a default dump to the database
    return true;
}

bool DatabaseModule::saveDump(QString file_path){
    return true;
}

bool DatabaseModule::restoreDump(){
    return true;
}

bool DatabaseModule::restoreDump(QString file_path){
    return true;
}

XmlConfig* DatabaseModule::loadDatabaseConfig(){
    XmlConfig *config_file{};

    if (!QFile::exists(DATABASE_DEFINITION_FILE))
        return nullptr;

    config_file = new XmlConfig(DATABASE_DEFINITION_FILE);
    if (!config_file->wellFormed()
            || !config_file->validateXmlWithDTD(DATABASE_CONFIG_DTD) ){
        delete config_file;
        return nullptr;
    }
    return config_file;
}

void DatabaseModule::handleFileChanged(const QString& file_name){
    if ( file_name ==  DATABASE_DEFINITION_FILE){
        emit configChanged();
    }
    //this is not really needed...
    else if ( file_name == DATABASE_CONFIG_DTD ){
    }
}
