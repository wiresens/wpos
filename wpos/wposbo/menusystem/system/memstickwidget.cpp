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


#include <QPushButton>
#include <QFrame>
#include <QLayout>
#include <QGroupBox>
#include <QListView>
#include <QPixmap>
#include <QStackedWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QHeaderView>
#include <QButtonGroup>
#include <QString>

#include <QStringList>
#include <QDateTime>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>
#include <QLineEdit>
#include <QFont>
#include <QLocale>

#include <iostream>
using namespace std;

#define TIMER_SLOT 5
#define INTERFACE_NAME "MemStickInterface"

MemStickWidget::MemStickWidget(QWidget *parent, const QString& name ):
    DCOPObject(INTERFACE_NAME),
    MemStickWidgetBase(parent,name),
    timer(0)
{

    timer = new QTimer(this);
    init();
}

MemStickWidget::~MemStickWidget(){
    delete timer;
}

void MemStickWidget::init(){
    QFont font;

    font = QApplication::font();
    font.setPointSize(16);
    db_listview->setFont(font);
    db_listview->clear();
    db_listview->setSorting(-1);
    db_listview->setColumnWidth(0,500);
    (db_listview->header())->hide();
    db_listview->setSelectionMode(QListView::Single);
    db_listview->setAllColumnsShowFocus(true);
    db_listview->setColumnAlignment(0,Qt::AlignCenter);



    export_icons_button->setIconSet(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/usb_devices.png"));
    import_icons_button->setIconSet(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/usb_devices.png"));
    export_sql_button->setIconSet(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/ddbb_copy1.png"));
    export_csv_button->setIconSet(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/ddbb_copy1.png"));
    import_db_button->setIconSet(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/ddbb_shrink.png"));
    stick_label->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/stick_48x48.png"));
    ok_button->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));
    accept_button->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));
    cancel_button->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_cancel.png"));


    connect(export_icons_button,SIGNAL(clicked()),this,SLOT(saveIconsToStickSlot()));
    connect(import_icons_button,SIGNAL(clicked()),this,SLOT(saveIconsFromStickSlot()));
    connect(export_sql_button,SIGNAL(clicked()),this,SLOT(saveDatabaseToStickSlot()));
    connect(export_csv_button,SIGNAL(clicked()),this,SLOT(saveDatabaseCSVToStickSlot()));
    connect(import_db_button,SIGNAL(clicked()),this,SLOT(getDatabaseFromStickClickedSlot()));

    connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelClicked()));
    connect(ok_button,SIGNAL(clicked()),this,SLOT(okClicked()));

    connect(db_name_lineedit,SIGNAL(textChanged(const QString&)),
            this,SLOT(dbNameChangedSlot(const QString&)));
    connect(db_listview,SIGNAL(selectionChanged(QListViewItem *)),
            this,SLOT(dbSelectionChangedSlot(QListViewItem *)));




    connect(accept_button,SIGNAL(clicked()),this,SLOT(acceptClickedSlot()));

    connectDCOPSignal(0,0,"stickOperationDone(bool)","operationResultSlot(bool)",false);
}


void MemStickWidget::saveIconsToStickSlot(){
    QString text;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);

    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }




    client = kapp->dcopClient();
    text = tr("Se esta procediendo con la copia de los iconos\
              Esta operacion puede durar unos minutos...");
              info_label->setText(text);
           startTimer();
    if (!client->send("dcopstick","dcopstick","asyncSaveIconsToStick()",data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
        QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    }
}

void MemStickWidget::saveIconsFromStickSlot(){
    QString text;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);


    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }



    client = kapp->dcopClient();
    text = tr("Se esta procediendo con la copia de los iconos\n\
              Esta operacion puede durar unos minutos...");
              info_label->setText(text);
           startTimer();
    if (!client->send("dcopstick","dcopstick","asyncGetIconsFromStick()",data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
        QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    }
}

void MemStickWidget::saveDatabaseToStickSlot(){
    QString text;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);

    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }



    client = kapp->dcopClient();
    text = tr("Se esta procediendo con la copia de seguridad de la base de datos");
    info_label->setText(text);
    startTimer();
    if (!client->send("dcopstick","dcopstick","asyncSaveDatabaseToStick()",data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
        QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    }
}

void MemStickWidget::saveDatabaseCSVToStickSlot(){
    QString text;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);

    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }



    client = kapp->dcopClient();
    text = tr("Se esta procediendo con la copia de seguridad en formato CSV de la base de datos");
    info_label->setText(text);
    startTimer();
    if (!client->send("dcopstick","dcopstick","asyncSaveDatabaseCSVToStick()",data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
        QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    }
}

void MemStickWidget::getDatabaseFromStickClickedSlot(){
    int i, count;
    QString sql_string;
    QStringList sql_list;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);
    QListViewItem *item = 0;

    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }

    clearDatabasePage();


    client = kapp->dcopClient();
    if (!client->call("dcopstick","dcopstick","getSqlFilesFromStick()",
                      data,reply_type,reply_data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        QString text = tr("Ha fallado la llamada al modulo de operaciones con memory stick .\n");
        QMessageBox::warning(this,tr("ha fallado la operacion de operaciones con stick"),text,0);
    }
    if (reply_type == "QString")
        reply >> sql_string;


    if (sql_string.isEmpty()){
        QString text = tr("No hay bases de datos guardadas en el Memory Stick.\n\n\
                          Puede que no sea posible acceder al memory Stick por que\n\
                          no este pinchado en una ranura USB o este ya montado");
                          QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
                       stack->raiseWidget(main_page);
        return;
    }

    stack->raiseWidget(select_db_page);
    qApp->processEvents();
    sql_list = QStringList::split(',',sql_string);
    count = sql_list.count();
    for(i=0;i<count;i++){
        //                cout << sql_list[i].toStdString() << endl;
        item = new QListViewItem(db_listview);
        item->setText(0,sql_list[i]);
        db_listview->insertItem(item);
    }

    ok_button->setEnabled(false);
}



void MemStickWidget::cancelClicked(){
    if (!checkStickModule())
        this->showErrorScreen();
    else
        stack->raiseWidget(main_page);
    this->clearDatabasePage();
}


void MemStickWidget::okClicked(){
    QString text;
    DCOPClient *client= 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data,IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);
    QListViewItem *item = 0;
    QString db_name;

    if (!checkStickModule()){
        this->showErrorScreen();
        return;
    }

    item = db_listview->selectedItem();
    db_name = db_name_lineedit->text();
    if ((!item)||(db_name.isEmpty())){
        stack->raiseWidget(main_page);
        return;
    }

    client = kapp->dcopClient();
    text = tr("Se esta procediendo con la copia de seguridad en formato CSV de la base de datos");
    info_label->setText(text);
    startTimer();
    out_stream << db_name;
    out_stream << item->text(0);
    if (!client->send("dcopstick","dcopstick","asyncSetDatabaseFromStick(QString,QString)",data)){
        cerr << "problems while trying to call dcopstick module" << endl;
        text = tr("Ha fallado la llamada al modulo de cargar del memory stick .\n");
        QMessageBox::warning(this,tr("Ha fallado la llamada al modulo de cargar del memory stick"),text,0);
    }
}

void MemStickWidget::dbSelectionChangedSlot(QListViewItem *item){
    this->checkAllDB();
}

void MemStickWidget::dbNameChangedSlot(const QString& name){
    this->checkAllDB();
}

void MemStickWidget::checkAllDB(){
    QListViewItem *item = 0;
    QString name;
    item = db_listview->selectedItem();
    name = db_name_lineedit->text();
    if ((item)&&(!name.isEmpty()))
        ok_button->setEnabled(true);
    else
        ok_button->setEnabled(false);
}





void MemStickWidget::operationResultSlot(bool status){
    QString text;

    if (!this->isVisible())
        return;

    if (status){
        this->stopTimer();
        stack->raiseWidget(all_ok_page);
    }
    else{
        this->stopTimer();
        cerr << "problemas al tratar de realizar operaciones con el modulo stick" << endl;
        text = tr("ha habido problemas al tratar de\n realizar la operacion en el memory stick .\n\
                  quiza el stick no este conectado, este mal conectado, o no este formateado\n");
                  QMessageBox::warning(this,tr("ha fallado la operacion con el stick"),text,0);
               stack->raiseWidget(main_page);
    }
}

void MemStickWidget::acceptClickedSlot(){
    if (!checkStickModule())
        this->showErrorScreen();
    else
        stack->raiseWidget(main_page);

}


void MemStickWidget::startTimer(){
    stack->raiseWidget(progress_page);
    qApp->processEvents();
    progress_bar->setTotalSteps(0);
    timer->start(TIMER_SLOT,false);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    pos = 0;
}

void MemStickWidget::timerSlot(){
    pos++;
    progress_bar->setProgress(pos);
    qApp->processEvents();
}

void MemStickWidget::stopTimer(){
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    timer->stop();
}

void MemStickWidget::showEvent(QShowEvent *e){
    if (!checkStickModule())
        this->showErrorScreen();
    else
        stack->raiseWidget(main_page);
    QWidget::showEvent(e);
}

bool MemStickWidget::showErrorScreen(){
    stack->raiseWidget(error_page);
    return true;
}


bool MemStickWidget::checkStickModule(){
    DCOPClient *client= 0;
    client = kapp->dcopClient();
    if (!client->isAttached())
        client->attach();

    //if the dcopreceipt is not running  exit without saving
    return (client->isApplicationRegistered("dcopstick"));
}

void MemStickWidget::clearDatabasePage(){
    db_listview->clear();
    db_name_lineedit->clear();
}

