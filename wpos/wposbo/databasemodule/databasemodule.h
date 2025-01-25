// file      :  databasemodule.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include "databasemoduleinterface.h"

#include <QtCore/QObject>

class QString;
class XmlConfig;
class QFileSystemWatcher;

class DatabaseModule : public QObject,
                       public DataBaseModuleInterface {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.DBusBOModel")

public:
    static const QString DBusObject;

    explicit DatabaseModule(QObject* parent = 0, const QString& name = QString {});
    ~DatabaseModule();

    // all this method are inherited from the DataBaseModuleInterface
    // the documentation is at the .h of this class

public slots:
    Q_SCRIPTABLE bool setConfig(QString xml_str) override;
    Q_SCRIPTABLE QString getConfig() override;
    Q_SCRIPTABLE void stopDatabase() override;
    Q_SCRIPTABLE void startDatabase() override;
    Q_SCRIPTABLE bool databaseState() override;
    Q_SCRIPTABLE bool compact() override;
    Q_SCRIPTABLE bool saveDump() override;
    Q_SCRIPTABLE bool saveDump(QString file_path) override;
    Q_SCRIPTABLE bool restoreDump() override;
    Q_SCRIPTABLE bool restoreDump(QString file_path) override;

signals:
    // all of this actions are done with the File Access Manager and QFileSystemWatcher
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
    QFileSystemWatcher* file_watcher;
};

#endif
