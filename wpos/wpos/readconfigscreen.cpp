/***************************************************************************
                          readconfigwidget.cpp  -  description
                             -------------------
    begin                : mi� may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "readconfigscreen.h"

#include <wposwidget/menupage.h>
#include <wposcore/genericsignalmanager.h>
#include <xmlconfig.h>

#include <QApplication>

#include <QLabel>
#include <QProgressBar>
#include <QScreen>

ReadConfigScreen::ReadConfigScreen(
    MenuPage *parent,
    const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    parent->addWidget(this, objectName());

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);
    setWindowFlag(Qt::Popup);
}

void ReadConfigScreen::showEvent(QShowEvent *e){
    progress->setValue(0);
    pos = 0;
    QWidget::showEvent(e);
}

void ReadConfigScreen::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    if (signal_name == GDATASIGNAL::REREAD_CONFIG){
        pos++;
        if ( xml ){
            type_label->setText(xml->readString("type"));
            first_id_label->setText(xml->readString("first_id"));
            last_id_label->setText(xml->readString("last_id"));
        }
        progress->setValue(pos);
        qApp->processEvents(QEventLoop::AllEvents);
    }
}
