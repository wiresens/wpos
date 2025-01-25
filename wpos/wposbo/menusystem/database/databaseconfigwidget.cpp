/***************************************************************************
                          bslddbbwidget.cpp  -  description
                             -------------------
    begin                : Tue Aug 19 2003
    copyright            : (C) 2003 by
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "databaseconfigwidget.h"
#include "databasemodule/databasemodule.h"

#include <libbslxml/xmlconfig.h>

#include <QApplication>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

extern "C" {
#include <stdlib.h>
}

#include <iostream>
using namespace std;

static const int TIME_INTERVAL = 10;
static const int WINDOW_HEIGHT = 680;
static const int FIELD = 0;

static const QString DDBB_CONFIGURATION_FILE { "xmldocs:bar_database.xml" };
static const QString DDBB_CONFIGURATION_DTD { "dtddocs:dbmodule_config.dtd" };
static const QString DDBB_CONFIGURATION_DTD_NMAP { "dtddocs:dbmodule_config_nmap.dtd" };
static const QString DDBB_FIND_SERVERS_PATH { "xmldocs:find_servers" };
static const QString DDBB_FIND_DATABASES_PATH { "xmldocs:find_databases" };
static const QString NMAP_RESULT_XML { "tmps:database_server.xml" }; // put the result of nmap here

DatabaseConfigWidget::DatabaseConfigWidget(
    DatabaseModule* dbmod,
    QWidget* parent,
    const QString& name)
    : QWidget(parent)
    , db_mod(dbmod)
{
    setupUi(this);
    setObjectName(name);
    setFixedHeight(WINDOW_HEIGHT); //@benes

    worker = new QProcess(this);
    timer = new QTimer(this);

    cancel_button->hide();
    ok_button->setEnabled(true);
    dbname_list_wgt->setEnabled(true);
    passwd_ledit->setEchoMode(QLineEdit::Password);
    scan_button->setText(tr("Find\nServers"));
    ok_button->setText(tr("Configure\nDatabase"));

    hostnames_list_wgt->setViewMode(QListView::ListMode);
    dbname_list_wgt->setViewMode(QListView::IconMode);

    server_pixmap_label->setPixmap(QPixmap("controls48:kcmsystem.png"));
    user_pixmap_label->setPixmap(QPixmap("controls48:personal.png"));
    database_pixmap_label->setPixmap(QPixmap("controls48:kcmdevices.png"));
    image_scan->setPixmap(QPixmap("controls48:info1.png"));

    cancel_button->setIcon(QPixmap("controls48:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));

    connect(user_name_ledit, &QLineEdit::textChanged, this, &DatabaseConfigWidget::userNameChanged);
    connect(hostnames_list_wgt, &QListWidget::itemSelectionChanged, this, &DatabaseConfigWidget::findDatabases);
    connect(dbname_list_wgt, &QListWidget::itemSelectionChanged, this, &DatabaseConfigWidget::dbNameChanged);

    connect(db_mod, &DatabaseModule::configChanged, this, &DatabaseConfigWidget::dbConfigChanged);

    connect(scan_button, &QPushButton::clicked, this, &DatabaseConfigWidget::scanButtonPressed);
    connect(ok_button, &QPushButton::clicked, this, &DatabaseConfigWidget::accept);
    connect(cancel_button, &QPushButton::clicked, this, &DatabaseConfigWidget::cancel);
}

void DatabaseConfigWidget::clear()
{
    user_name_ledit->clear();
    passwd_ledit->clear();
    dbname_list_wgt->clear();
    hostnames_list_wgt->clear();
}

void DatabaseConfigWidget::cancel()
{
    // action to be done at the cancel action
    clear();
    getHostConfig();
}

void DatabaseConfigWidget::scanButtonPressed()
{
    cancel();
    findServers();
}

void DatabaseConfigWidget::accept()
{
    auto db_item = dbname_list_wgt->currentItem();
    auto host_item = hostnames_list_wgt->currentItem();

    if (user_name_ledit->text().isEmpty() || !db_item || !host_item) {
        QString text = tr("Not all fields are correct. It is necessary to provide a database of \n a specific server to be able to proceed with configuring the wpos program");
        QMessageBox::warning(this, tr("Trying to save configuration file"), text, QMessageBox::NoButton);
        return;
    }

    XmlConfig xml;
    xml.createElementSetDomain("database");
    xml.createElement("hostname", host_item->text());
    xml.createElement("dbname", db_item->text());
    xml.createElement("user", user_name_ledit->text());
    xml.createElement("passwd", passwd_ledit->text());
    xml.delDomain();

    if (!xml.validateXmlWithDTD(DDBB_CONFIGURATION_DTD_NMAP)) {
        QString text = tr(" The XML file %1 of the database\n does not validate with DTD. Check the content and try again");
        QMessageBox::warning(this, tr("Invalid Database's XML File"), text.arg(NMAP_RESULT_XML), QMessageBox::NoButton);
        return;
    }

    saveReportmanDeff(host_item->text(), db_item->text(), user_name_ledit->text(), passwd_ledit->text());
    if (!db_mod->setConfig(xml.toString())) {
        QString text = tr("Problems while trying to save database settings.\n It is possible that the configuration is incorrect, or \n communication with the remote server has been interrupted");
        QMessageBox::warning(this, tr("Trying to save configuration file"), text, QMessageBox::NoButton);
        return;
    }
}

bool DatabaseConfigWidget::saveReportmanDeff(const QString& host, const QString& db_name, const QString& user, const QString& passwd)
{

    QDir dir = QDir::home();
    if (!dir.cd(".borland")) {
        cout << "the directory borland does not exist at" << dir.path().toStdString() << endl;
        return false;
    }

    QFile file(dir.path() + "/dbxconnections");
    if (file.exists()) {
        if (!file.remove()) {
            cout << "reportman file dbxconnections can not be deleted" << endl;
            return false;
        }
    }
    if (!file.open(QIODevice::WriteOnly)) {
        cout << "Reportman dbxconnections cant be created" << endl;
        return false;
    }

    QTextStream stream(&file);
    stream << "[caja]\n";
    stream << "DriverName=ZeosLib\n";
    stream << "Database Protocol=postgresql\n";
    stream << "HostName=" << host.toLatin1() << "\n";
    stream << "Database=" << db_name.toLatin1() << "\n";
    stream << "User_Name=" << user.toLatin1() << "\n";
    stream << "Password=" << passwd.toLatin1() << "\n";
    stream << "Port=5432\n";
    stream << "Zeos Translsolation=None\n";
    stream << "Property1=\n";
    stream << "Property2=\n";
    stream << "Property3=\n";
    stream << "Property4=\n";
    stream << "Property5=\n";
    stream << "Property6=\n";
    stream << "Property7=\n";
    stream << "Property8=\n";
    stream << "Property9=\n";

    file.close();
    return true;
}

void DatabaseConfigWidget::userNameChanged(const QString& /*text*/)
{
    findDatabases();
}

void DatabaseConfigWidget::dbNameChanged() { }

void DatabaseConfigWidget::dbConfigChanged()
{
    QString text = tr("The database configuration has changed\n The data will be updated to the current configuration\\n");
    QMessageBox::warning(this, tr("New database configuration"), text, QMessageBox::NoButton);
    clear();
    getHostConfig();
}

void DatabaseConfigWidget::findServers()
{
    scan_button->setEnabled(false);

    // Set wait cursor while discovering and others elements to off state
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ddbb_user_frame->setEnabled(false);
    dbname_list_wgt->setEnabled(false);
    stack->setCurrentWidget(wait);

    // it should be run with root id... (it runs nmap)
    worker->setProgram(DDBB_FIND_SERVERS_PATH);
    worker->setArguments(QStringList(NMAP_RESULT_XML));

    connect(worker, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DatabaseConfigWidget::endFindServers);
    connect(timer, &QTimer::timeout, this, &DatabaseConfigWidget::setProgressBar);
    prog_int = 0;
    timer->start(TIME_INTERVAL);
    worker->start();
}

void DatabaseConfigWidget::endFindServers(int exitCode, QProcess::ExitStatus exitStatus)
{
    timer->stop();
    scan_button->setEnabled(true);
    QApplication::restoreOverrideCursor();
    if (exitStatus != QProcess::NormalExit || exitCode == 255)
        return;

    disconnect(worker, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DatabaseConfigWidget::endFindServers);
    disconnect(timer, &QTimer::timeout, this, &DatabaseConfigWidget::setProgressBar);

    XmlConfig xml(NMAP_RESULT_XML);
    if (!xml.wellFormed())
        return;

    QStringList hostnames;
    for (auto i = 0; i < xml.howManyTags("host"); i++) {
        xml.setDomain("host[" + QString::number(i) + "]");
        if (xml.readAttribute("status", "state") == "up" && xml.readAttribute("ports.port.state", "state") == "open") {
            auto hostname = xml.readAttribute("hostnames.hostname", "name");
            if (hostname.isEmpty())
                hostname = xml.readAttribute("address", "addr");
            hostnames += hostname;
        }
        xml.releaseDomain("host");
    }

    stack->setCurrentWidget(servers);
    showValues(hostnames, hostnames_list_wgt, QPixmap("controls48:kcmsystem.png"));

    // Set normal cursor and others elements to on state when discovering has finnished
    // QApplication::setOverrideCursor(QCursor(ArrowCursor));
    ddbb_user_frame->setEnabled(true);
    dbname_list_wgt->setEnabled(true);
}

void DatabaseConfigWidget::findDatabases()
{
    scan_button->setEnabled(false);
    //    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    auto source = qobject_cast<QListWidget*>(sender());
    QList<QListWidgetItem*> items;
    if (!source || (items = source->selectedItems()).isEmpty() || user_name_ledit->text().isEmpty())
        return;

    auto selected_item = items.first();
    auto current_item = hostnames_list_wgt->currentItem();

    if (!current_item || current_item == selected_item)
        return;
    current_item = selected_item;

    timer->stop();
    dbname_list_wgt->clear();

    auto dbname = current_item->text();
    auto username = user_name_ledit->text();

    QStringList args { username, dbname, NMAP_RESULT_XML };
    worker->setProgram(DDBB_FIND_DATABASES_PATH);
    worker->setArguments(args);

    connect(worker, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DatabaseConfigWidget::endFindDatabases);
    connect(timer, &QTimer::timeout, this, &DatabaseConfigWidget::setProgressBar);

    stack->setCurrentWidget(wait);
    progress_bar->setValue(0);
    prog_int = 0;
    timer->start(TIME_INTERVAL);
    worker->start(); // it should be run with root id... (it runs nmap)
}

void DatabaseConfigWidget::endFindDatabases(int exitCode, QProcess::ExitStatus exitStatus)
{
    timer->stop();
    scan_button->setEnabled(true);
    //    QApplication::restoreOverrideCursor();
    if (exitStatus != QProcess::NormalExit || exitCode == 255)
        return;

    disconnect(worker, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &DatabaseConfigWidget::endFindDatabases);
    disconnect(timer, &QTimer::timeout, this, &DatabaseConfigWidget::setProgressBar);

    XmlConfig xml(NMAP_RESULT_XML);
    if (!xml.wellFormed())
        return;

    QStringList hostnames;
    for (auto i = 0; i < xml.howManyTags("name"); i++)
        hostnames += xml.readString("name[" + QString::number(i) + "]");

    stack->setCurrentWidget(servers);
    showValues(hostnames, dbname_list_wgt, QPixmap("controls48:kcmdevices.png"));
}

void DatabaseConfigWidget::setProgressBar()
{
    progress_bar->setValue(prog_int++);
}

void DatabaseConfigWidget::showValues(
    const QStringList& captions,
    QListWidget* list_wgt,
    QPixmap icon)
{
    auto pos = 0;
    //    list_wgt->clear();
    for (auto elem : captions) {
        auto item = new QListWidgetItem(list_wgt);
        item->setText(elem);
        item->setIcon(icon);
        list_wgt->insertItem(pos++, item);
    }
}

bool DatabaseConfigWidget::getHostConfig()
{
    if (!db_mod)
        return false;
    clear();

    XmlConfig xml;
    xml.readXmlFromString(db_mod->getConfig());
    if (!xml.wellFormed() || !xml.validateXmlWithDTD(DDBB_CONFIGURATION_DTD))
        return false;

    stack->setCurrentWidget(servers);
    user_name_ledit->setText(xml.readString("database.user"));
    passwd_ledit->setText(xml.readString("database.passwd"));

    QStringList hostnames;
    hostnames += xml.readString("database.hostname");
    showValues(hostnames, hostnames_list_wgt, QPixmap("controls48:kcmsystem.png"));

    hostnames.clear();
    hostnames.append(xml.readString("database.dbname"));
    showValues(hostnames, dbname_list_wgt, QPixmap("controls48:kcmdevices.png"));
    return true;
}

void DatabaseConfigWidget::showEvent(QShowEvent* e)
{
    getHostConfig();
    findServers();
    QWidget::showEvent(e);
}

void DatabaseConfigWidget::hideEvent(QHideEvent* e)
{
    if (worker->state() == QProcess::Running)
        worker->terminate();
    timer->stop();
    QWidget::hideEvent(e);
}
