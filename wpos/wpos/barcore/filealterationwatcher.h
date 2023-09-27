/***************************************************************************
                          filealterationwatcher.h  -  description
                             -------------------
    begin                : dom may 9 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef FILEALTERATIONWATCHER_H
#define FILEALTERATIONWATCHER_H

#include <QObject>
#include "filealterationwatcherinterface.h"

class QString;
class XmlConfig;
class QFileSystemWatcher;

class FileAlterationWatcher :
        public QObject,
        public NAlterationWatcherInterface{

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wpos.ConfigFilesWatcher")

    static const QString DBusObject;

public:
    explicit FileAlterationWatcher(QObject *parent = nullptr,
                          const QString& name = QString());

    ~FileAlterationWatcher();

public slots:
    void startWatching();
    void stopWatching();
    void genericSignalSlot(const QString& signal);

    Q_SCRIPTABLE void initDownButtonGroupSlot();
    Q_SCRIPTABLE void initUpButtonGroupSlot();
    Q_SCRIPTABLE void initProductsSlot();
    Q_SCRIPTABLE void initOffersSlot();
    Q_SCRIPTABLE void initDevicesSlot();

protected slots:
    void fileDirtySlot(const QString& file_name);
    void productFileSlot();
    void buttonFileSlot();
    void offersSlot();
    void devicesSlot();

    void raiseMainWidget();
    void raiseConfigWidget();

    void realButton();
    void realProduct();
    void realOffer();
    void realDevices();

signals:
    void genericSignal(const QString& signal_name);
    void genericDataSignal(const QString& signal_name, XmlConfig* xml);

private:
    QFileSystemWatcher* watcher{};
    bool on{false};
};
#endif
