/***************************************************************************
                          genericsignalmanager.cpp  -  description
                             -------------------
    begin                : jue jul 10 2003
    copyright            : (C) 2003 by Napsis S.L.
    email                : support@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "genericsignalmanager.h"
#include <xmlconfig.h>

#include <QString>
#include <QStringList>
#include <QDebug>

// Simple alternative using C++11
GenericSignalManager* GenericSignalManager::instance(){
    static GenericSignalManager gsm;
    return &gsm;
}

void GenericSignalManager::publishGenericSignal(
    const QString& signal,
    QObject *publisher)
{

    if (!publisher) return;

    ObjectHub* publisher_hub  { m_signal_publishers.find(signal) };

    if (publisher_hub){    //the signal already registered by another object
        publisher_hub->objs.append(publisher, publisher->objectName());
//        qDebug() << "Publication of SIGNAL  : ** " << signal << "** of " << publisher->objectName() << Qt::endl;
    }
    else{       //the signal is not yet registered by any object
        publisher_hub = new ObjectHub;
        publisher_hub->shared_signal = signal;
        publisher_hub->objs.append(publisher, publisher->objectName());
        m_signal_publishers.append(publisher_hub, publisher_hub->shared_signal);
//        qDebug() << "New Publication of SIGNAL : ** " << signal << "** of " << publisher->objectName() << Qt::endl;
    }

    //connect with all the sloted_objs
    ObjectHub* subscriber_hub = m_signal_subscribers.find(signal);

    if (!subscriber_hub) return;

    for (auto& subscriber : subscriber_hub->objs){
        connect(publisher, SIGNAL(genericSignal(const QString&)),
                subscriber, SLOT(genericSignalSlot(const QString&)));

//        qDebug() << "Connect SIGNAL -- : **" << signal
//                 << "** publisher : " << publisher->objectName()
//                 << " subscriber : " << subscriber->objectName()
//                 << Qt::endl;
    }
}

void GenericSignalManager::publishGenericDataSignal(
    const QString& signal,
    QObject *publisher)
{

    if (!publisher)  return;

    ObjectHub* publisher_hub = m_data_signal_publishers.find(signal);

    if (publisher_hub){     //the signal already exists
        publisher_hub->objs.append(publisher, publisher->objectName());
//        qDebug() << "Publication of DATA SIGNAL : ** " << signal << "** of " << publisher->objectName() << Qt::endl;
    }
    else{     //the signal does not exists
        publisher_hub = new ObjectHub;
        publisher_hub->shared_signal = signal;
        publisher_hub->objs.append(publisher, publisher->objectName());
        m_data_signal_publishers.append(publisher_hub, publisher_hub->shared_signal);
//        qDebug() << "New Publication of DATA SIGNAL : ** " << signal << "** of " << publisher->objectName() << Qt::endl;
    }

    //connect with all the attached_list
    ObjectHub* subs_hub = m_data_signal_subscribers.find(signal);
    if (!subs_hub)  return;

    for (auto &subscriber : subs_hub->objs){

        connect(publisher, SIGNAL(genericDataSignal(const QString&, XmlConfig *)),
                subscriber, SLOT(genericDataSignalSlot(const QString&, XmlConfig *)));

//        qDebug() << "Connect DATA SIGNAL -- : " << signal
//                 << " publisher : " << publisher->objectName()
//                 << " subscriber : " << subscriber->objectName()
//                 << Qt::endl;
    }
}

void GenericSignalManager::unpublishGenericSignal(
    const QString& signal,
    QObject *publisher)
{

    //we need the postition to remove at the end
    ObjectHub* publisher_hub = m_signal_publishers.find(signal);
    if (!publisher_hub)  return;

    auto tmp_publisher = publisher_hub->objs[publisher->objectName()];
    if (tmp_publisher){
        tmp_publisher->disconnect(SIGNAL(genericSignal(const QString&)));
//        qDebug() << "Disconnect SIGNAL **" << signal
//                 << "** publisher : " << tmp_publisher->objectName()
//                 << Qt::endl;
        publisher_hub->objs.remove(); //This might be a bug. I thing we should call remove(i)
    }

    //check if it's the last node
    if (publisher_hub->objs.isEmpty()){
        m_signal_publishers.remove(signal);
//        qDebug() << "Last Node, removing SIGNAL **" << signal << "**" << Qt::endl;
    }
}

void GenericSignalManager::unpublishGenericDataSignal(
    const QString& signal,
    QObject *publisher)
{

    //the position is needed to remove at the end
    ObjectHub *publisher_hub = m_data_signal_publishers.find(signal);
    if (!publisher_hub) return;

    auto tmp_publisher = publisher_hub->objs[publisher->objectName()];
    if (tmp_publisher){
        tmp_publisher->disconnect(SIGNAL(genericDataSignal(const QString&, XmlConfig* )));

//        qDebug() << "Disconnect DATA SIGNAL **" << signal
//                 << "** publisher : " << tmp_publisher->objectName()
//                 << Qt::endl;

        publisher_hub->objs.remove(); //This might be a bug. I thing we should call remove(i)
    }

    //check if it's the last node
    if (publisher_hub->objs.isEmpty()){
        m_data_signal_publishers.remove(signal);
//        qDebug() << "Last Node, removing DATA SIGNAL **" << signal << "**" << Qt::endl;
    }
}

void GenericSignalManager::subscribeToGenericSignal(
    const QString& signal,
    QObject *subscriber)
{

    if (!subscriber) return;

    //working with attached_list;
    ObjectHub *subscriber_hub = m_signal_subscribers.find(signal);

    if (!subscriber_hub){
        //there was no attached node
        subscriber_hub = new ObjectHub;
        subscriber_hub->shared_signal = signal;
        subscriber_hub->objs.append(subscriber, subscriber->objectName());
        m_signal_subscribers.append(subscriber_hub, subscriber_hub->shared_signal);

//        qDebug() << "New Subscrition to SIGNAL : ** " << signal << "** by " << subscriber->objectName() << Qt::endl;
    }
    else{
        subscriber_hub->objs.append(subscriber, subscriber->objectName());
//        qDebug() << "Subscrition to SIGNAL : ** " << signal << "** by " << subscriber->objectName() << Qt::endl;
    }

    subscriber_hub = m_signal_publishers.find(signal);
    if (!subscriber_hub)  return;

    for(auto* publisher : subscriber_hub->objs){
        connect(publisher, SIGNAL(genericSignal(const QString&)),
                subscriber, SLOT(genericSignalSlot(const QString&)));

//        qDebug() << "Connect SIGNAL via Subscription : **" << signal
//                 << "** publisher : " << publisher->objectName()
//                 << " subscriber : " << subscriber->objectName()
//                 << Qt::endl;
    }
}

void GenericSignalManager::subscribeToGenericDataSignal(
    const QString& signal,
    QObject *subscriber)
{

    if (!subscriber) return;

    //working with attached_list;
    ObjectHub *subscriber_hub = m_data_signal_subscribers.find(signal);
    if (!subscriber_hub){
        //there was no attached node
        subscriber_hub = new ObjectHub;
        subscriber_hub->shared_signal = signal;
        subscriber_hub->objs.append(subscriber, subscriber->objectName());
        m_data_signal_subscribers.append(subscriber_hub, subscriber_hub->shared_signal);
//        qDebug() << "New Subscrition to DATA SIGNAL : ** " << signal << "** by " << subscriber->objectName() << Qt::endl;
    }
    else{
        subscriber_hub->objs.append(subscriber,subscriber->objectName());
//        qDebug() << "Subscrition to DATA SIGNAL : ** " << signal << "** by " << subscriber->objectName() << Qt::endl;
    }

    subscriber_hub = m_data_signal_publishers.find(signal);
    if (!subscriber_hub)  return;

    for(auto* publisher : subscriber_hub->objs){
        connect(publisher, SIGNAL(genericDataSignal(const QString&, XmlConfig*)),
                subscriber, SLOT(genericDataSignalSlot(const QString&, XmlConfig*)));

//        qDebug() << "Connect DATA SIGNAL via Subscription : **" << signal
//                 << "** publisher : " << publisher->objectName()
//                 << " subscriber : " << subscriber->objectName()
//                 << Qt::endl;
    }
}

void GenericSignalManager::unsubscribeToGenericSignal(
    const QString& signal,
    QObject *subscriber)
{

    ObjectHub *subscriber_hub =  m_signal_subscribers.find(signal);
    if (!subscriber || !subscriber_hub ) return;

    int i{};
    if ( (i = findObjectAtSignalName(subscriber, signal, &m_signal_subscribers) ) == -1 )
        return ;

    //delete object from subscribers list
    subscriber_hub->objs.remove(i);

    auto publisher_hub = m_signal_publishers.find(signal);
    if (!publisher_hub)  return;

    for(auto* publisher : publisher_hub->objs){
        subscriber->disconnect(publisher, SIGNAL(genericSignal(const QString&)));
//        qDebug() << "Disconnect SIGNAL via subscription **" << signal
//                 << "** publisher : " << publisher->objectName() << Qt::endl;
    }
}

void GenericSignalManager::unsubscribeToGenericDataSignal(
    const QString& signal,
    QObject *subscriber)
{

    ObjectHub *subscriber_hub = m_signal_subscribers.find(signal);
    if ( !subscriber || !subscriber_hub )  return;

    int i{};
    if ( (i = findObjectAtSignalName(subscriber, signal,   &m_signal_subscribers)) == -1 )
        return;

    //delete object from attached list
    subscriber_hub->objs.remove(i);

    auto publisher_hub = m_data_signal_publishers.find(signal);
    if (!publisher_hub)   return;
    for(auto* publisher : publisher_hub->objs){
        subscriber->disconnect(publisher, SIGNAL(genericDataSignal(const QString&, XmlConfig *)) );
//        qDebug() << "Disconnect DATA SIGNAL via subscription **" << signal
//                 << "** publisher : " << publisher->objectName() << Qt::endl;
    }
}

int GenericSignalManager::findObjectAtSignalName(
    QObject *obj,
    const QString& signal,
    HList<ObjectHub> *cluster_list)
{
    ObjectHub *registered_hub{};
    if ( !cluster_list || !( registered_hub = cluster_list->find(signal) ) ) return -1;
    return  registered_hub->objs.indexOf(obj);
}
