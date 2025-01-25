/***************************************************************************
                          memstickwidget.cpp  -  description
                             -------------------
    begin                : lun jul 26 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "memstickwidget.h"
#include "memstickwidget_adaptor.h"
#include "ui_memstickwidgetbase.h"

#include <QMessageBox>
#include <QtDBus/QDBusConnection>

#include <iostream>
using namespace std;

static const uint TIMER_SLOT { 5 };
static const QString iconsMsg { QObject::tr("The icons are being copied\n This operation may take a few minutes...") };
static const QString dbMsg { QObject::tr("Database backup is in progress...") };
static const QString csvMsg { QObject::tr("The backup copy of the database in CSV format is in progress...") };
static const QString failedMsg { QObject::tr("The call to the memory stick save module has failed") };
static const QString failedcaption { QObject::tr("The save operation on the stick has failed") };

const QString MemStickWidget::DBusObject = QString { "/wpos/wposbo/MemStick" };

MemStickWidget::MemStickWidget(
    QWidget* parent,
    const QString& name)
    : QWidget(parent)
{
    ui = new Ui::MemStickWidgetBase();
    ui->setupUi(this);
    setObjectName(name);
    new MemStickAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(MemStickWidget::DBusObject, this);

    timer = new QTimer(this);
    init();
}

MemStickWidget::~MemStickWidget()
{
    delete timer;
}

void MemStickWidget::init()
{

    QFont font = QApplication::font();
    font.setPointSize(16);
    ui->db_listview->setFont(font);
    ui->db_listview->clear();
    ui->db_listview->setSortingEnabled(false);
    ui->db_listview->setColumnWidth(0, 500);
    ui->db_listview->header()->hide();
    ui->db_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->db_listview->setAllColumnsShowFocus(true);
    //    ui->db_listview->setColumnAlignment(0, Qt::AlignCenter);

    ui->export_icons_button->setIcon(QPixmap("controls32:usb_devices.png"));
    ui->import_icons_button->setIcon(QPixmap("controls32:usb_devices.png"));
    ui->export_sql_button->setIcon(QPixmap("controls32:ddbb_copy1.png"));
    ui->export_csv_button->setIcon(QPixmap("controls32:ddbb_copy1.png"));
    ui->import_db_button->setIcon(QPixmap("controls32:ddbb_shrink.png"));
    ui->stick_label->setPixmap(QPixmap("controls48:stick_48x48.png"));
    ui->ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    ui->accept_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    ui->cancel_button->setIcon(QPixmap("controls48:button_cancel.png"));

    connect(ui->export_icons_button, &QPushButton::clicked, this, &MemStickWidget::saveIconsToStick);
    connect(ui->import_icons_button, &QPushButton::clicked, this, &MemStickWidget::loadIconsFromStick);
    connect(ui->export_sql_button, &QPushButton::clicked, this, &MemStickWidget::saveDatabaseToStick);
    connect(ui->export_csv_button, &QPushButton::clicked, this, &MemStickWidget::saveDatabaseCSVToStick);
    connect(ui->import_db_button, &QPushButton::clicked, this, &MemStickWidget::getDatabaseFromStick);

    connect(ui->cancel_button, &QPushButton::clicked, this, &MemStickWidget::cancel);
    connect(ui->ok_button, &QPushButton::clicked, this, &MemStickWidget::proceed);

    connect(ui->db_name_lineedit, &QLineEdit::textChanged, this, &MemStickWidget::dbName);
    connect(ui->db_listview, &QTreeWidget::itemSelectionChanged, this, &MemStickWidget::dbSelection);

    connect(ui->accept_button, &QPushButton::clicked, this, &MemStickWidget::accept);

    //    connectDCOPSignal(0,0, "stickOperationDone(bool)", "operationResultSlot(bool)",false); @benes
}

void MemStickWidget::saveIconsToStick()
{
    QString text;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    //@benes    client = kapp->dcopClient();
    ui->info_label->setText(iconsMsg);
    startTimer();
    //@benes    if (!client->send("dcopstick","dcopstick", "asyncSaveIconsToStick()", data))
    errorMessage(failedcaption, iconsMsg, std::cerr);
}

void MemStickWidget::loadIconsFromStick()
{
    QString text;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    //  client = kapp->dcopClient();  @benes
    ui->info_label->setText(iconsMsg);
    startTimer();
    //    if (!client->send("dcopstick", "dcopstick","asyncGetIconsFromStick()",data)) @benes
    errorMessage(failedcaption, iconsMsg, std::cerr);
}

void MemStickWidget::saveDatabaseToStick()
{
    QString text;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    //    client = kapp->dcopClient(); @benes
    ui->info_label->setText(dbMsg);
    startTimer();
    //    if (!client->send("dcopstick","dcopstick","asyncSaveDatabaseToStick()",data)) @benes
    errorMessage(failedcaption, failedMsg, std::cerr);
}

void MemStickWidget::saveDatabaseCSVToStick()
{
    QString text;
    QString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    //    client = kapp->dcopClient(); @benes
    ui->info_label->setText(csvMsg);
    startTimer();
    //    if (!client->send("dcopstick","dcopstick","asyncSaveDatabaseCSVToStick()",data))  @benes
    errorMessage(failedcaption, failedMsg, std::cerr);
}

void MemStickWidget::getDatabaseFromStick()
{
    int i, count;
    QString sql_string;
    QStringList sql_list;
    QString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    clearDatabasePage();

    //    client = kapp->dcopClient(); @benes
    //    if (!client->call("dcopstick","dcopstick","getSqlFilesFromStick()", data,reply_type,reply_data)) @benes
    errorMessage(failedcaption, failedMsg, std::cerr);
    if (reply_type == "QString")
        reply >> sql_string;

    if (sql_string.isEmpty()) {
        QString text = tr(
            "There are no databases saved on the Memory Stick.\n\n\
            The memory stick may not be accessible because\n\
            It is not plugged into a USB slot or it is already mounted");
        errorMessage(failedcaption, text, std::cerr);
        ui->stack->setCurrentWidget(ui->main_page);
        return;
    }

    ui->stack->setCurrentWidget(ui->select_db_page);
    qApp->processEvents();
    sql_list = sql_string.split(QLatin1Char(','));
    count = sql_list.count();

    for (auto elt : sql_list) {
        auto item = new QTreeWidgetItem(ui->db_listview);
        item->setText(0, elt);
    }

    ui->ok_button->setEnabled(false);
}

void MemStickWidget::cancel()
{
    if (!stickModuleAvailable())
        showErrorScreen();
    else
        ui->stack->setCurrentWidget(ui->main_page);
    this->clearDatabasePage();
}

void MemStickWidget::proceed()
{
    QString text;
    QString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);
    QString db_name;

    if (!stickModuleAvailable()) {
        showErrorScreen();
        return;
    }

    auto selected_items = ui->db_listview->selectedItems();
    if (selected_items.isEmpty())
        return;

    db_name = ui->db_name_lineedit->text();
    if (selected_items.isEmpty() || db_name.isEmpty()) {
        ui->stack->setCurrentWidget(ui->main_page);
        return;
    }
    auto item = selected_items.at(0);
    //    client = kapp->dcopClient();
    ui->info_label->setText(csvMsg);
    startTimer();
    out_stream << db_name;
    out_stream << item->text(0);

    //    if (!client->send("dcopstick","dcopstick", "asyncSetDatabaseFromStick(QString,QString)",data)) @benes
    errorMessage(failedcaption, failedMsg, std::cerr);
}

void MemStickWidget::dbSelection()
{
    checkAllDB();
}

void MemStickWidget::dbName(const QString& name)
{
    checkAllDB();
}

void MemStickWidget::checkAllDB()
{

    auto selected_items = ui->db_listview->selectedItems();
    auto name = ui->db_name_lineedit->text();

    if (!selected_items.isEmpty() && !name.isEmpty())
        ui->ok_button->setEnabled(true);
    else
        ui->ok_button->setEnabled(false);
}

void MemStickWidget::setOperationResult(bool status)
{

    if (!isVisible())
        return;

    if (status) {
        stopTimer();
        ui->stack->setCurrentWidget(ui->all_ok_page);
    } else {
        stopTimer();
        auto msg = tr(
            "There have been problems trying to\nperform the operation on the memory stick.\n\
            Maybe the stick is not connected, connected incorrectly or it is not formatted\n");
        errorMessage(failedcaption, msg, std::cerr);
        ui->stack->setCurrentWidget(ui->main_page);
    }
}

void MemStickWidget::accept()
{
    if (!stickModuleAvailable())
        showErrorScreen();
    else
        ui->stack->setCurrentWidget(ui->main_page);
}

void MemStickWidget::startTimer()
{
    ui->stack->setCurrentWidget(ui->progress_page);
    qApp->processEvents();
    ui->progress_bar->setValue(0);
    timer->start(TIMER_SLOT);
    connect(timer, &QTimer::timeout, this, &MemStickWidget::timerSlot);
    pos = 0;
}

void MemStickWidget::timerSlot()
{
    pos++;
    ui->progress_bar->setValue(pos);
    qApp->processEvents();
}

void MemStickWidget::stopTimer()
{
    disconnect(timer, &QTimer::timeout, this, &MemStickWidget::timerSlot);
    timer->stop();
}

void MemStickWidget::showEvent(QShowEvent* event)
{
    /*if ( !stickModuleAvailable())  showErrorScreen();
    else*/
    ui->stack->setCurrentWidget(ui->main_page);
    QWidget::showEvent(event);
}

bool MemStickWidget::wposStckicRunning()
{
    //    client = kapp->dcopClient();
    //    if (!client->isAttached()) client->attach();

    //    if the dcopreceipt is not running  exit without saving
    //    return (client->isApplicationRegistered("dcopstick"));
    return false; // @benes
}

bool MemStickWidget::showErrorScreen()
{
    ui->stack->setCurrentWidget(ui->error_page);
    return true;
}

bool MemStickWidget::stickModuleAvailable()
{
    return wposStckicRunning();
}

void MemStickWidget::clearDatabasePage()
{
    ui->db_listview->clear();
    ui->db_name_lineedit->clear();
}

void MemStickWidget::errorMessage(
    const QString& caption,
    const QString& msg,
    ostream& s)
{
    s << "Problems while trying to call wposstick module\n";
    s << "Make sure the process wposstick is running an try again\n";
    QMessageBox::warning(this, caption, msg, QMessageBox::NoButton);
}
