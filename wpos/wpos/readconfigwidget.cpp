/***************************************************************************
                          readconfigwidget.cpp  -  description
                             -------------------
    begin                : mi� may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#include "readconfigwidget.h"
#include "barcommon.h"
#include "signals_definition.h"
#include <genericsignalmanager.h>
#include <xmlconfig.h>

#include <QApplication>

#include <QLabel>
#include <QProgressBar>

extern GenericSignalManager *generic_signal_manager;

ReadConfigWidget::ReadConfigWidget(QWidget *parent, const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    generic_signal_manager->attachToGenericDataSignal(GDATASIGNAL_REREAD_CONFIG,this);
}

ReadConfigWidget::~ReadConfigWidget(){}

void ReadConfigWidget::showEvent(QShowEvent */*e*/){
    progress->setValue(0);
    pos = 0;
}

void ReadConfigWidget::genericDataSignalSlot(const QString& signal_name, XmlConfig *xml){
    if (signal_name == GDATASIGNAL_REREAD_CONFIG){
        pos++;
        if (xml){
            type_label->setText(xml->readString("type"));
            first_id_label->setText(xml->readString("first_id"));
            last_id_label->setText(xml->readString("last_id"));
        }
        progress->setValue(pos);
        qApp->processEvents();
    }
}
