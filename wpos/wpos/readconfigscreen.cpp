/***************************************************************************
                          readconfigwidget.cpp  -  description
                             -------------------
    begin                : mi� may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "ui_readconfigwidget.h"
#include "readconfigscreen.h"

#include <wposgui/windows/menupage.h>
#include <wposcore/genericsignalmanager.h>
#include <libbslxml/xmlconfig.h>

#include <QApplication>

#include <QLabel>
#include <QProgressBar>
#include <QScreen>

ReadConfigScreen::ReadConfigScreen(
    MenuPage *parent,
    const QString& name ):
    QWidget(parent),
    ui{ new Ui::ReadConfigWidget}
{
    ui->setupUi(this);
    setObjectName(name);
    parent->addWidget(this, objectName());

    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::REREAD_CONFIG, this);
    setWindowFlag(Qt::Popup);
}

void ReadConfigScreen::showEvent(QShowEvent *e){
    ui->progress->setValue(0);
    m_pos = 0;
    QWidget::showEvent(e);
}

void ReadConfigScreen::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    if (signal_name == GDATASIGNAL::REREAD_CONFIG){
        m_pos++;
        if ( xml ){
            ui->type_label->setText(xml->readString("type"));
            ui->first_id_label->setText(xml->readString("first_id"));
            ui->last_id_label->setText(xml->readString("last_id"));
        }
        ui->progress->setValue(m_pos);
        qApp->processEvents(QEventLoop::AllEvents);
    }
}
