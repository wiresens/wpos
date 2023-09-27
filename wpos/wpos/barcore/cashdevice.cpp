/***************************************************************************
                          cashbox.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "cashdevice.h"

#include <wposcore/genericsignalmanager.h>
#include <xmlconfig.h>
#include <QFile>
#include <QString>

extern "C"{
#include <stdlib.h>
#include <stdio.h>
}

#include <iostream>

namespace std{}
using namespace std;

extern QString CASHBOX_DEVICE;
extern QString CASHBOX_TYPE;

CashDevice::CashDevice(QObject *parent, const QString& name ):
    QObject(parent)
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericSignal(GSIGNAL::OPEN_CASHBOX, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG, this);
}

CashDevice::~CashDevice(){}

void CashDevice::initCashDevice(){}

void CashDevice::genericSignalSlot(const QString& signal_name){

    if (signal_name == GSIGNAL::OPEN_CASHBOX){

        open(CASHBOX_DEVICE, CASHBOX_TYPE);
        XmlConfig xml;
        xml.createElement("event_type", "open_cashbox");
        emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml);
      }
}

bool CashDevice::open(const QString& cashbox_dev, const QString& cashbox_type){
    const char* device_node = cashbox_dev.toStdString().c_str();
    FILE *device = fopen(device_node, "w");

    auto error_msg = "CashDevice::genericSignalSlot( " + GSIGNAL::OPEN_CASHBOX.toStdString() +" )";
    error_msg += std::string("Problem while opening the Cash Box device at node " + std::string(device_node));

    if (!device) std::perror(error_msg.c_str());
    else{
        if ( cashbox_type == "cash_drawer"  || cashbox_type == "p_samsung_350" ){
            char c[] = { 0x1b, 0x70, 0x00, 0x30, 0x30 };
            fwrite(c, sizeof(char), 5 , device);
        }
        else{
            char c[] = { 0x0A };
            fwrite(c, sizeof(char) , 1, device);
        }

        fflush(device);
        fclose(device);
        qDebug("CashDevice::genericSignalSlot -- CashBox Opened and Closed Succesfully");
    }
    return !device;
}
