/***************************************************************************
                          fx2000enrollwidget.cpp  -  description
                             -------------------
    begin                : Sat oct 25 2003
    copyright            : (C) 2003 by BSTeam
    email                : bsteam@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "fx2000enrollwidget.h"

#include <libbslxml/xmlconfig.h>
#include <wposwidget/poslabel.h>

#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QIcon>
#include <QTabWidget>
#include <QTimer>
#include <QFile>
#include <QLayout>
#include <QProgressBar>
#include <QLocale>

const QString& RH   {"hands:r-hand.png"};
const QString& RH_0 {"hands:r-hand0.png"};
const QString& RH_1 {"hands:r-hand1.png"};
const QString& RH_2 {"hands:r-hand2.png"};
const QString& RH_3 {"hands:r-hand3.png"};
const QString& RH_4 {"hands:r-hand4.png"};
const QString& LH   {"hands:l-hand.png"};
const QString& LH_0 {"hands:l-hand0.png"};
const QString& LH_1 {"hands:l-hand1.png"};
const QString& LH_2 {"hands:l-hand2.png"};
const QString& LH_3 {"hands:l-hand3.png"};
const QString& LH_4 {"hands:l-hand4.png"};
const QString& EMPTY_FINGERPRINT {"hands:black_fingerprint.png"};
const QString& FINGERPRINT_DCOP_MODULE {"dcopfx2000"};

#include <iostream>
using namespace std;

FX2000EnrollWidget::FX2000EnrollWidget(QWidget *parent, const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    //init the fingerprint device fx2000 section.
    QHBoxLayout *llayout = new QHBoxLayout(l_hand_frame);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    left_hand_label = new PosLabel(LH, l_hand_frame);
    llayout->addWidget(left_hand_label);
    left_hand_label->show();

    QHBoxLayout *rlayout = new QHBoxLayout(r_hand_frame);
    rlayout->setMargin(0);
    rlayout->setSpacing(0);
    right_hand_label = new PosLabel( RH, r_hand_frame);
    rlayout->addWidget(right_hand_label);
    right_hand_label->show();

    left_hand_label->insert(LH_0,10,62,25,35);
    left_hand_label->insert(LH_1,28,14,20,37);
    left_hand_label->insert(LH_2,60,7,30,30);
    left_hand_label->insert(LH_3,90,18,30,30);
    left_hand_label->insert(LH_4,117,37,30,30);

    right_hand_label->insert(RH_4,9,41,30,30);
    right_hand_label->insert(RH_3,31,18,27,30);
    right_hand_label->insert(RH_2,57,9,30,30);
    right_hand_label->insert(RH_1,89,14,30,30);
    right_hand_label->insert(RH_0,108,62,35,35);

    connect(left_hand_label, SIGNAL(clickAt(QString)), this, SLOT(doubleClickedAtLeft(QString)));
    connect(right_hand_label, SIGNAL(clickAt(QString)), this, SLOT(doubleClickedAtRight(QString)));
    connect(acquire_button, SIGNAL(released()), this, SLOT(fingerprintAcquireSlot()));
}

FX2000EnrollWidget::~FX2000EnrollWidget(){
    delete right_hand_label;
    delete left_hand_label;
}

void FX2000EnrollWidget::setId(QString name){
    if ( name.isEmpty() ) return;
    id = name;
}

QString FX2000EnrollWidget::getId(){
    return id;
}

void FX2000EnrollWidget::setReturnApplication(QString _app_name, QString _interface_name, QString _method_name)
{
    app_name = _app_name;
    interface_name = _interface_name;
    method_name = _method_name;
}

void FX2000EnrollWidget::fingerprintAcquireSlot(){

    if ( finger_number == -1 ){
        finger_label->setText(tr("You must select a finger of a hand\nbefore acquiring a print"));
        QTimer::singleShot(4000, this, SLOT(clearFingerLabel()));
        return;
    }

    //Acquire fingerprint... the slot method of the user Module will sen us a signal to show the reults
    //of the acquire action.
////    client = kapp->dcopClient();
////    if ( !client->isAttached() || !client->isApplicationRegistered(FINGERPRINT_DCOP_MODULE)) return;

    //give the application name and also the interface to communicate with the results of the fingerprint
    //acquire process

    cout  << "return application ";
    cout << app_name.toStdString() << " ";
    cout << interface_name.toStdString() << " ";
    cout << method_name.toStdString() << endl;

    QByteArray array1;
    QByteArray array2;
    QDataStream stream1(&array1, QIODevice::ReadWrite);
    stream1 << app_name;
    stream1 << interface_name;
    stream1 << method_name;

////@benes if ( !client->send("dcopfx2000", "FX2000Enroll", "setReturnApplication(QString,QString,QString)",array1) )
    {
        QString text = tr("An error occurred while trying to insert the user's fingerprint\nwith id  %1 It is possible that an internal dcopserver error has occurred or the \ncommunication with the dcopserver has been broken.\n").arg(id);
        QMessageBox::warning(this,tr("Failed to insert user fingerprint"),text, QMessageBox::NoButton);
        return;
    }

    QDataStream stream2(&array2, QIODevice::ReadWrite);
    stream2 << id;
    stream2 << finger_number; //it should be take from the fingerprint

////@benes if ( !client->send("dcopfx2000","FX2000Enroll","asyncEnroll(QString,int)", array2) )
    {
        QString text = tr("An error occurred while trying to insert the user's fingerprint\nwith id  %1 It is possible that an internal dcopserver error has occurred or the \ncommunication with the dcopserver has been broken.\n").arg(id);
        QMessageBox::warning(this, tr("Failed to insert user fingerprint"), text, QMessageBox::NoButton);
        return;
    }
}

void FX2000EnrollWidget::fingerprintReadSlot(QString xml_str){

    XmlConfig xml;
    xml.readXmlFromString(xml_str);
    auto id = xml.readString("identity");
    auto quality = xml.readString("quality").toFloat();
    auto fingerprint = xml.readString("finger");
    auto fingerprint_path = xml.readString("fingerprint_path");

    //show the fingerprint and all the other things
    fingerprint_pixmap->setPixmap(QPixmap(fingerprint_path));
    int q = (int) ( quality*100.0 );
    quality_bar->setValue(q);

    QString text = tr("La huella se guardo correctamente\n");
    finger_label->setText(text);
    QTimer::singleShot(10000, this, SLOT(clearFingerLabel()));

}

void FX2000EnrollWidget::clearFingerLabel(){
    finger_label->clear();
}

void FX2000EnrollWidget::doubleClickedAtLeft(QString name){
    left_hand_label->setPixmap(QPixmap(name));
    right_hand_label->setPixmap(QPixmap(RH));

    QString text = tr("Press the acquire button to save the fingerprint\ncorresponding to the selected finger");
    finger_label->setText(text);

    if (name == LH_0) finger_number = 0;
    else if (name == LH_1) finger_number = 1;
    else if (name == LH_2) finger_number = 2;
    else if (name == LH_3) finger_number = 3;
    else if (name == LH_4) finger_number = 4;
    QTimer::singleShot(4000,this,SLOT(clearFingerLabel()));
}

void FX2000EnrollWidget::doubleClickedAtRight(QString name){
    right_hand_label->setPixmap(QPixmap(name));
    left_hand_label->setPixmap(QPixmap(LH));
    QString text = tr("Press the acquire button to save the fingerprint\ncorresponding to the selected finger");
    finger_label->setText(text);

    if (name == RH_0) finger_number = 5;
    else if (name == RH_1) finger_number = 6;
    else if (name == RH_2) finger_number = 7;
    else if (name == RH_3) finger_number = 8;
    else if (name == RH_4) finger_number = 9;
    QTimer::singleShot(4000, this, SLOT(clearFingerLabel()));
}

void FX2000EnrollWidget::showEvent(QShowEvent *e){
    right_hand_label->setPixmap(QPixmap(RH));
    left_hand_label->setPixmap(QPixmap(LH));
    fingerprint_pixmap->setPixmap(QPixmap());
    quality_bar->setValue(100);
    QWidget::showEvent(e);
}
