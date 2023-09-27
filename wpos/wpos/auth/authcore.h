/***************************************************************************
                          authcore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef _AUTHCORE_H_
#define _AUTHCORE_H_

#include <QObject>
#include <xmlconfig.h>

class QString;
class AuthCoreDB;
class UserData;
class XmlConfig;

class AuthCore: public QObject{
    Q_OBJECT
public:
    explicit AuthCore(QObject *parent = nullptr, QString name = QString{});
    ~AuthCore();

    QString getUserId();
    QString getUserName();
    QString getUserLastName();
    QString getFullName();
    bool getAdmin();

    bool setUserName(QString name);
    bool setUserId(QString id);
    bool setUserLastName(QString last_name);

    bool clearUser();

public slots:
    void genericDataSignalSlot(const QString& signal_name, XmlConfig *_xml);

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);

protected:
    void emitUserChanged();
    void processXml(XmlConfig *_xml);

    AuthCoreDB *db{};
    UserData *user_data_ptr{};
};

#endif
