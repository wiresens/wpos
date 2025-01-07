/***************************************************************************
                          genericmenubutton.cpp  -  description
                             -------------------
    begin                : jul 23 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "genericmenubutton.h"
#include <wposcore/genericsignalmanager.h>

#include <libbslxml/xmlconfig.h>

#include <QString>
#include <QFile>

const QString GENERIC_BUTTON {"tmps:generic_menu_button.xml"};

GenericMenuButton::GenericMenuButton(const QString& text, QWidget *parent, const QString& name ) :
    QPushButton(text, parent),
    r_list{new QList<RMenuButtonData*>}
{
    setObjectName(name);

    QFile file(GENERIC_BUTTON);
    if (file.exists()) file.remove();

    xml = new XmlConfig(GENERIC_BUTTON);
    xml->delDomain();

    connect(this, &GenericMenuButton::released, this, &GenericMenuButton::releasedSlot);
}

void GenericMenuButton::releasedSlot(){
    int count, i;
    RMenuButtonData *data = 0;

    count = r_list->count();
    xml->delDomain();
    for(i=0; i<count; i++){
        data = r_list->at(i);
        while (xml->howManyTags("name"))
            xml->deleteElement("name");
        xml->createElement("name",data->data);
        emit this->genericDataSignal(data->signal_name,xml);
    }
}

void GenericMenuButton::createRelationship(const QString& signal_name,const QString& _data){
    RMenuButtonData *data = 0;
    data = new RMenuButtonData;
    data->data = _data;
    data->signal_name = signal_name;

    r_list->append(data);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(data->signal_name,this);
}

void GenericMenuButton::clearRelationships(){
    int count, i;
    RMenuButtonData *data = 0;

    auto gsm = GenericSignalManager::instance();
    count = r_list->count();
    for(i=0;i<count;i++){
        data = r_list->at(i);
        gsm->unpublishGenericSignal(data->signal_name,this);
    }
    r_list->clear();
}
