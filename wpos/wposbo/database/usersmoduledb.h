// file      :  usersmoduledb.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef USERSMODULEDB_H
#define USERSMODULEDB_H

#include <QList>
#include <libbslxml/xmlconfig.h>
#include <wposcore/basicdatabase.h>

class QString;
class XmlConfig;

template <typename T>
class QList;
template <typename Key, typename Value>
class QMap;

template <typename Value>
using QPtrMap = QMap<QString, Value*>;

template <typename Value>
using QPtrList = QList<Value*>;

struct UserData {
    QString employee_id;
    QString name;
    QString last_name;
    QString address;
    QString nss;
    QString phone;
    QString cellular;
    QString email;
    QString company_id;
    QString picture;
    bool gerente {};
    bool active {};
    bool isEmpty() const;
    void clear();
};

class UsersModuleDB : public BasicDatabase {
public:
    /**
     *       constructor, the connection name should be different for each connection, the rest of parametters are:
     *       @param  QString the name of the connection. Each connection must have different name.
     *       @param  QString with the hostname of the host that have the database.
     *       @param  QString with the name of the database.
     *       @param  QString with the name of the user to connect to the database.
     *       @param  QString with the password to make operations at the database.
     */
    UsersModuleDB(const QString& _connection_name, const QString& _hostname,
        const QString& _database, const QString& _username, const QString& _passwd);
    /**
     *       constructor, the connection name should be different for each connection. the XmlConfig should describe
     *       the connection.
     *       @param  QString the name of the connection. Each connection must have different name.
     *       @param  XmlConfig with the description of the connection. The Xml Should validate against the following DTD
     *
     *       <!ELEMENT AUTOMATICALLY_GENERATED_XML (databse)>
     *       <!ELEMENT database (hostname,dbname,user,passwd)>
     *       <!ELEMENT hostname (#PCDATA)>
     *       <!ELEMENT dbname (#PCDATA)>
     *       <!ELEMENT user (#PCDATA)>
     *       <!ELEMENT passwd (#PCDATA)>
     */
    UsersModuleDB(const QString& _connection_name, XmlConfig* xml);

    /**
     *       constructor, the connection name should be different for each connection. the XmlConfig should describe
     *       the connection.
     *       @param  QString the name of the connection. Each connection must have different name.
     *       @param  A path to an Xml with the description of the connection.
     *       The Xml Should validate against the basicDatabase Connection DTD
     */
    UsersModuleDB(const QString& _connection_name, const QString& configuration_path);
    ~UsersModuleDB();

    /**
     *       add a user to the database
     *       @param UserData. the id,name,last_name of the userData should not be empty
     *       @return bool with true if the user has been inserted at the databse, false if errors.
     */
    bool addUser(const UserData* user);

    /**
     *       this method tries to update the user with the user->employee_id, with all the data at the UserData class
     *       @param UserData. the id,name,last_name of the userData should not be empty, the employee_id
     *       select the user that is going to be updated.
     *       @return bool with true if the user has been updated at the databse, false if errors.
     */
    bool updateUser(const UserData* user);

    /**
     *       this method delte a user from the database
     *       @param QString with the employe_id to delete from the database
     *       @return true if the user has been deleted from the database, or false if errors.
     */
    bool delUser(const QString& id);
    bool realDelUser(const QString& id);

    /**
     *       this method get the complete list of the users at the database.
     *       @return Null if errors ocurrs or A pointer to QPtrList<UserData> if there are no problems...
     */
    QVector<UserData> getUserList(bool all = false);

    /**
     *       this method is used to get the information of a user
     *       @param QString with the employee_id of the user.
     *       @return UserData with all the information of that user or null if error
     */
    UserData getUserInfo(QString employee_id);

    /**
     *       this method is used to set at the acl table permissions
     *       to the user given as parameter.
     *       @param QString with the employee_id of the user
     *       @param boolean with true if the permission should be set on
     *       @return true if there where no problems...false if problems
     *       ocurs or the permission was set previously.
     */
    bool setUserAdminPermission(const QString& employee_id, bool permission);
    bool getUserAdminPermission(const QString& employee_id);

    /**
     *       this method is used to set at the active flag to true or false
     *       @param QString with the employee_id of the user
     *       @param boolean with true if the user should be active or false if inactive
     */
    bool getActiveFlag(const QString& employee_id);
    bool setActiveFlag(const QString& employee_id, bool flag = true);
};

#endif
