/***************************************************************************
                          databasemodule.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include "databasemoduleinterface.h"

#include <QtCore/QObject>

class QString;
class XmlConfig;
class QFileSystemWatcher;

class DatabaseModule :
        public QObject,
        public DataBaseModuleInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOModel")

public:
    static const QString DBusObject;

    explicit DatabaseModule(QObject *parent = 0, const QString& name = QString());
    ~DatabaseModule();

    //all this method are inherited from the DataBaseModuleInterface
    //the documentation is at the .h of this class

public slots:
    bool setConfig(QString xml_str) override;
    QString getConfig() override;
    void stopDatabase() override;
    void startDatabase() override;
    bool databaseState() override;
    bool compact() override;
    bool saveDump() override;
    bool saveDump(QString file_path) override;
    bool restoreDump() override;
    bool restoreDump(QString file_path) override;

signals:
    //all of this actions are done with the File Access Manager and QFileSystemWatcher
    void configChanged();
    void stateChanged();
    void compactDone();
    void restoreDone();
    void dumpDone();

protected slots:
    void handleFileChanged(const QString&);

protected:
    XmlConfig* loadDatabaseConfig();

protected:
    QFileSystemWatcher *file_watcher;
};

#endif
