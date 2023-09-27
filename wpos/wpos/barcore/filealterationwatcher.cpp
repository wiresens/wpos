/***************************************************************************
                          filealterationwatcher.cpp  -  description
                             -------------------
    begin                : dom may 9 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "filealterationwatcher.h"
#include "filealterationwatcher_adaptor.h"

#include "mainscreen.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include "filemanager.h"
#include <xmlconfig.h>

#include <QDataStream>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QDebug>

#include <iostream>

using namespace std;

extern QString CASHBOX_DEVICE;
extern QString CASHBOX_TYPE;

extern FileManager *file_manager;

#define INTERFACE_NAME "ReadConfigInterface"
#define DELAY 1000

const QString FileAlterationWatcher::DBusObject  = QString{"/wpos/wpos/ConfigFilesWatcher"};

FileAlterationWatcher::FileAlterationWatcher(QObject *parent, const QString& name):
    QObject(parent)
{
    setObjectName(name);

    new ConfigFilesWatcherAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(FileAlterationWatcher::DBusObject, this);

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_DEVICES, this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_PRODUCTS, this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_BUTTONS, this);
    gsm->publishGenericSignal(GSIGNAL::LOAD_OFFERS, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);

    watcher = new QFileSystemWatcher(this);
    watcher->addPath(Files::configFilePath("buttons"));
    watcher->addPath(Files::configFilePath("database"));
    watcher->addPath( Files::configFilePath("invitations"));
    watcher->addPath( Files::configFilePath("orders"));
    watcher->addPath(Files::configFilePath("devices"));

    // connectDCOPSignal(0,0,"optionChanged()","initUpButtonGroupSlot()",false);
}

FileAlterationWatcher::~FileAlterationWatcher(){
    stopWatching();
}

void FileAlterationWatcher::startWatching(){
    connect(watcher, SIGNAL(fileChanged(const QString&)),
            this, SLOT(fileDirtySlot(const QString&)));
}

void FileAlterationWatcher::stopWatching(){

    disconnect(watcher, SIGNAL(fileChanged(const QString&)),
               this, SLOT(fileDirtySlot(const QString&)));
}

void FileAlterationWatcher::fileDirtySlot(const QString& file_name){

    stopWatching();
    qDebug() << file_name.toLatin1() << " Has been modified" ;
    if (file_name == Files::configFilePath("bar"))
        QTimer::singleShot(DELAY, this, SLOT(productFileSlot()));

    else if (file_name == Files::configFilePath("buttons"))
        QTimer::singleShot(DELAY, this, SLOT(buttonFileSlot()));

    else if (file_name == Files::configFilePath("invitations"))
        QTimer::singleShot(DELAY, this, SLOT(offersSlot()));

    else if (file_name == Files::configFilePath("devices"))
        QTimer::singleShot(DELAY, this, SLOT(devicesSlot()));

    if (file_manager)  file_manager->saveFiles();
}

void FileAlterationWatcher::productFileSlot(){
    realProduct();
    startWatching();
}

void FileAlterationWatcher::buttonFileSlot(){
    realButton();
    startWatching();
}

void FileAlterationWatcher::offersSlot(){
    realOffer();
    startWatching();
}

void FileAlterationWatcher::devicesSlot(){
    realDevices();
    startWatching();
}

void FileAlterationWatcher::initDownButtonGroupSlot(){
    realButton();
}

void FileAlterationWatcher::initUpButtonGroupSlot(){
    realButton();
}

void FileAlterationWatcher::initProductsSlot(){
    realProduct();
}

void FileAlterationWatcher::initOffersSlot(){
    offersSlot();
}

void FileAlterationWatcher::initDevicesSlot(){
    devicesSlot();
}

void FileAlterationWatcher::raiseMainWidget(){
    XmlConfig xml ;
    xml.createElement("name", MainScreen::LOGIN_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void FileAlterationWatcher::raiseConfigWidget(){
    XmlConfig xml;
    xml.createElement("name", MainScreen::READ_CONFIG_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void FileAlterationWatcher::realButton(){

    cout << " Customizing Buttons " << endl;
    XmlConfig xml(Files::configFilePath("buttons"));

    if (xml.isValid()){
        raiseConfigWidget();
        emit genericSignal(GSIGNAL::LOAD_BUTTONS);
        raiseMainWidget();
    }
    else
        cout << " Problems with the XML " << endl;
}

void FileAlterationWatcher::realProduct(){
    cout << " rereading product screens " << endl;
    XmlConfig xml(Files::configFilePath("bar"));
    if (xml.isValid()){
        raiseConfigWidget();
        emit genericSignal(GSIGNAL::LOAD_PRODUCTS);
        raiseMainWidget();
    }
    else
        cout << " Problems with the xml " << endl;
}

void FileAlterationWatcher::realOffer(){
    cout << " Rereading offers " << endl;
    XmlConfig xml (Files::configFilePath("invitations"));
    if (xml.isValid()){
        raiseConfigWidget();
        emit genericSignal(GSIGNAL::LOAD_OFFERS);
        raiseMainWidget();
    }
    else
        cout << " Problems with the xml" << endl;
}

void FileAlterationWatcher::realDevices(){

    cout << " Rereading devices " << endl;
    XmlConfig xml (Files::configFilePath("devices"));
    if (xml.isValid()){
        QString cashbox_type { xml.readString("cashbox.type") };
        QString cashbox_device { xml.readString("cashbox.dev") };

        if ( cashbox_type.isEmpty() || cashbox_device.isEmpty()) return;

        CASHBOX_TYPE = cashbox_type;
        CASHBOX_DEVICE = cashbox_device;
    }
    else
        cout << " Problems with the xml" << endl;
}

void FileAlterationWatcher::genericSignalSlot(const QString& signal){
    if (signal == GSIGNAL::LOAD_DEVICES) realDevices();
}
