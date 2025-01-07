/***************************************************************************
                          databasemoduleinterface.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef _DATABASE_MODULE_INTERFACE_H_
#define _DATABASE_MODULE_INTERFACE_H_

#include <QString>

class DataBaseModuleInterface{

    /**
        *       this method should be used to change the configuration of the database at this machine.
        *       The database configuration is readed from the file at xmldocs:bar_database.xml.
        *       All the database objects at both programs (ntpv and ntpvbackoffice) are designed
        *       to read this file and connect with this databse, so all the actions of this module will be
        *       performed to the database at the config.
        *
        *       the xmldocs:bar_database.xml must have the following scheme:
        *<?xml version = '1.0' encoding = 'UTF-8'?>
        *<AUTOMATICALLY_GENERATED_XML>
        *             <database>
        *                     <hostname>192.168.1.3</hostname>
        *                     <dbname>caja</dbname>
        *                     <user>fewa</user>
        *                     <passwd></passwd>
        *            </database>
        *</AUTOMATICALLY_GENERATED_XML>
        *
        *       wich means that it could be validated with a DTD. The DTD of this scheme should be at
        *       dtddocs:dbmodule_config.dtd.
        *<!ELEMENT AUTOMATICALLY_GENERATED_XML (database)>
        *<!ELEMENT database (hostname,dbname,user,passwd)>
        *<!ELEMENT hostname (#PCDATA)>
        *<!ELEMENT dbname (#PCDATA)>
        *<!ELEMENT user (#PCDATA)>
        *<!ELEMENT passwd (#PCDATA)>
        *
        *       @param QString containing an XML with the configuration of the database. With this string the module will
        *       construct a valid XML and will try to set the config of each program to that database.
        *
        *       @return bool with false, if the database can't be written because of a dtd validation failure, or the dtd is missing, or
        *       the program can't write at the xmldocs:bar_xml. TRUE if the configuration have been done without problems
        */
    virtual bool setConfig(QString) = 0;

    /**
        *       this method will return the configuration of this POS. The configuration is at the file "xmldocs:bar_database.xml".
        *       it will be readed from that file and returned with this method.
        *
        *       @return QString containing an XML with the configuration of the database connection at this ntpv and ntpvbarconfig
        *       program. If there are problems while trying to obtain the database configuration it will return an empty QString.
        *       QString can construct a XmlConfig object wich should validate against " dtddocs:dbmodule_config.dtd"
        *       of to a dtd like at the setConfig method. It could be validated as follows:
        *
        *       QString aux;    //with aux containing the xml config.
        *       XmlConfig *xml = new XmlConfig();
        *       xml->readXmlFromString(aux);
        *       if (xml->ok()){
        *               if (xml->validateXmlWithDTD(DTD))
        *                       cout << "the xml has been validated against the config dtd"
        *       }
        */
    virtual QString getConfig() = 0;

    /**
        *       this method will try to stop the database at the config of this POS.
        */
    virtual  void stopDatabase() = 0;

    /**
        *       this method will try to start the database of the config at this POS
        */
    virtual  void startDatabase() = 0;

    /**
        *       state of the current database at this POS.
        *       @return this method will return true if the database and the database service contained at the xmldocs:bar_database.xml
        *       of this POS is accesible for the user at the same config file. it will return false otherwise.
        */
    virtual bool databaseState() = 0;

    /**
        *       this method will do a vacuum at the database.
        */
    virtual bool compact() = 0;

    /**
        *       this method will dump all the data and structures of the database at the default location for the distributed program
        *       (by now /var/ntpvbackoffice/data).
        *
        *       @return true if the dump has been donde at the correct file or false if some problems ocurs
        */
    virtual bool saveDump() = 0;

    /**
        *       the same like above method but in this case the location of the file could be defined by an external parameter
        *       it will save the dump of the database at the file given as parameter
        *
        *       @param QString containing a path to the file where the dump is going to be saved.
        *       @return true if the dump has been donde at the correct file or false if some problems ocurs
        */
    virtual bool saveDump(QString) = 0;

    /**
        *       method to restore a dumped database to the current database configuration.
        *       it will take the file with the dumps from the default location, and will try to restore all the data and
        *       structures from it at the current database configuration.
        *
        *       @return true if the database has been restored with the dump at the default location.
        */
    virtual bool restoreDump() = 0;

    /**
        *       the same like above method, but in this case the location of the file could be defined with an external parameter
        *       it will try to restore a database from a dump file.
        *
        *       @param QString with a path of the file where the database is going to be restored.
        */
    virtual bool restoreDump(QString) = 0;
};


#endif
