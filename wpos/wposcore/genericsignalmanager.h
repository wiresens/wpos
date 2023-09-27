/***************************************************************************
                          genericsignalmanager.h  -  description
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

#ifndef GENERICSIGNALMANAGER_H
#define GENERICSIGNALMANAGER_H

/**
*    Generic signal manager Class tries to provide a flexible way to connect objects with generic signals.
*    This class has all the logic to mantain the relationship between the object that emits the signals
*    and the objects that receives the signals.
*    @author Carlos Manzanedo
*/

#include <QObject>

#include "hlist.h"
#include "signals.h"

class QString;

class GenericSignalManager : public QObject  {
    Q_OBJECT

public:
    GenericSignalManager(const GenericSignalManager&) = delete;
    GenericSignalManager& operator=(const GenericSignalManager&) = delete;

    static GenericSignalManager* instance();

private:
    GenericSignalManager()  = default;
    ~GenericSignalManager() = default;

    static GenericSignalManager* gsm;
    struct GenericSignalHub{
        QString shared_signal;
        HList<QObject> objs;
    };

public slots:
    bool publishGenericSignal(const QString& signal, QObject *publisher);
    bool publishGenericDataSignal(const QString& signal, QObject *publisher);
    bool unpublishGenericSignal(const QString& signal, QObject *publisher);
    bool unpublishGenericDataSignal(const QString& signal, QObject *publisher);

    bool subscribeToGenericSignal(const QString& signal, QObject *subscriber);
    bool subscribeToGenericDataSignal(const QString& signal, QObject *subscriber);
    bool unsubscribeToGenericSignal(const QString& signal, QObject *subscriber);
    bool unsubscribeToGenericDataSignal(const QString& signal, QObject *subscriber);

protected:    
    int findObjectAtSignalName(QObject *object,
                               const QString& signal,
                               HList<GenericSignalHub> *cluster_list);

    HList<GenericSignalHub> m_signal_publishers;
    HList<GenericSignalHub> m_signal_subscribers;
    HList<GenericSignalHub> m_data_signal_publishers;
    HList<GenericSignalHub> m_data_signal_subscribers;
};

#endif
