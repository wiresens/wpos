/***************************************************************************
                          bslexitactionswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "exitactionswidget.h"

#include <wposcore/genericsignalmanager.h>
#include "salesscreen.h"

#include <xmlconfig.h>

#include "barcore/filemanager.h"

#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QApplication>

#include <iostream>
using namespace std;

extern FileManager *file_manager;

ExitActionsWidget::ExitActionsWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    initDialog();

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG,this);
    gsm->publishGenericSignal(GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE,this);


    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls:button_ok_48.png"));
    connect(ok_button, &QPushButton::clicked, this, &ExitActionsWidget::shutdownSlot);
    connect(cancel_button, &QPushButton::clicked, this, &ExitActionsWidget::returnToADMmenuSlot);
}

ExitActionsWidget::~ExitActionsWidget(){}

void ExitActionsWidget::returnToADMmenuSlot(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void ExitActionsWidget::shutdownSlot(){

    XmlConfig xml;
    xml.createElement("event_type","shutdown");
    emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml);

    emit genericSignal(GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE);
    file_manager->saveFiles();
    system("sudo /sbin/sync &");
    system("sudo /sbin/shutdown -h now &");
    exit (0);
}

void ExitActionsWidget::initDialog(){

    auto dateTime = QDateTime::currentDateTime();
    main_title_label->setText(tr("ATTENTION: It will turn off the box"));
    auto abstract = tr("By clicking on the accept button, the box will turn off.<br><br>\
                  Make sure you have made the Z corresponding to today (<font color=\"yellow\">\
                  %1 </font> at <font color=\"yellow\"> %2 </font>).Otherwise the Z will drag\
                  to the next opening of the box.<br><br> Are you sure of <u>want to turn off \
                  box</u>?").arg(dateTime.date().toString()).arg(dateTime.time().toString());                                                                                                                                       abstract_label->setText(abstract);
}
