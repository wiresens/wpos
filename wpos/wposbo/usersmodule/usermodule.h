/***************************************************************************
                          usermodule.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef USERMODULE_H
#define USERMODULE_H

#include "usermoduleinterface.h"
#include <QtCore/QObject>
#include <QList>

class QString;
class UserData;

class UserModule :
        public QObject ,
        public UserModuleInterface {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOUser")

public:
    static const QString DBusObjectPath;

    explicit UserModule(QObject *parent=0, const QString& name = QString());
    ~UserModule() = default;

    QString getInterfaceName();
    QString getEnrollMethodName();

    /**
    * For this method check comments at usermodule interface.
    */

public slots:
    bool createUser(const QString&);
    bool changeUser(const QString&);
    bool deleteUser(const QString&);
    bool realDelUser(const QString&);
    QString usersList(bool all=false);
    QString usersExtendedList(bool all=false);
    QString getUserInfo(const QString& employee_id);
    void enrollUserData(const QString& xml);

    bool delUserFingerprints(const QString&);

signals:
    /**
        *this signal is emited when a user has been created.
        *@param QString with the employee_id of the user that has been created
        */
    void userCreated(QString);

    /**
        *this signal is emited when a user has been deleted.
        *@param QString with the employee_id of the user that has been deleted
        */
    void userDeleted(QString);

    /**
        *       this signal is emited when a user has been Updated.
        *       @param QString with the employee_id of the user that has been updated
        */
    void userUpdated(QString);

    /**
        *       this signal is emmited when a change ocurs at the databasee.
        *       @param QString with the employee_id of the user that has been created
        */
    void updatesAvailable();

    /**
        *       this signal is emmited when a correct data has arrived from the fx2000 enroll
        *       interface
        *       @param QString containing the xml validated against /etc/ntpv/dtds/dcopfx2000_enroll.dtd
        *       the data has been validated so be sure it's correct.
        */
    void enrollDataAvailable(QString);

protected:
    UserData readUserFromXml(const QString& string_xml);
};

#endif
