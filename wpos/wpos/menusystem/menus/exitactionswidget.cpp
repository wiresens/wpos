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

#include <libbslxml/xmlconfig.h>

#include "barcore/filemanager.h"

#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QApplication>

extern FileManager *file_manager;

ExitActionsWidget::ExitActionsWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    initializeDialogBox();

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE,this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG,this);
    gsm->publishGenericSignal(GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE,this);


    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls:button_ok_48.png"));
    connect(ok_button, &QPushButton::clicked, this, &ExitActionsWidget::shutdown);
    connect(cancel_button, &QPushButton::clicked, this, &ExitActionsWidget::returnToADMmenuSlot);
}

ExitActionsWidget::~ExitActionsWidget(){}

void ExitActionsWidget::returnToADMmenuSlot(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::ADMIN_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SET_PAGE, &xml);
}

void ExitActionsWidget::shutdown(){

    XmlConfig xml;
    xml.createElement("event_type","shutdown");
    emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml);

    emit genericSignal(GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE);
    file_manager->saveFiles();
    system("sudo /sbin/sync &");
    system("sudo /sbin/shutdown -h now &");
    exit (0);
}

void ExitActionsWidget::initializeDialogBox(){
    auto dateTime = QDateTime::currentDateTime();
    main_title_label->setText(tr("Warning: You are about to turn off this POS"));
    auto abstract = tr("By clicking on the accept button, the box will turn off.<br><br>\
                  Make sure you have made the Zeta corresponding of today's (<font color=\"yellow\">\
                  %1 </font> operations at <font color=\"yellow\"> %2 </font>).Otherwise the Zeta will drag\
                  to the next opening of the box.<br><br> Are you sure of <u>want to turn off \
                  the POS</u>?").arg(dateTime.date().toString()).arg(dateTime.time().toString());                                                                                                                                       abstract_label->setText(abstract);
}
