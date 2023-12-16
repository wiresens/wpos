/***************************************************************************
                          authcore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef AUTHCORE_H_
#define AUTHCORE_H_

#include "database/authcoredb.h"
#include <QObject>
#include <xmlconfig.h>

class QString;
class AuthCoreDB;
class UserData;
class XmlConfig;

class AuthCore: public QObject{
    Q_OBJECT

public:
    explicit AuthCore(
        QObject *parent = nullptr,
        QString name = QString()
    );

    ~AuthCore();

    const QString& userId() const;
    const QString& userName() const;
    const QString& userLastName() const;
    QString userFullName() const;
    bool isRootUser() const;

    bool loadUserById(QString id);

    //Potential bug : Name and Last are not unique identifiers
    bool loadUserByName(QString name);
    bool loadUserByLastName(QString last_name);

public slots:
    void genericDataSignalSlot(const QString& signal, XmlConfig *xml);

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected:
    void notifyUserChanged();
    void processXml(XmlConfig& xml);

private:
    static AuthCoreDB db;
    UserData user;
};

#endif
