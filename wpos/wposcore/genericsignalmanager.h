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

#ifndef GENERIC_SIGNAL_MANAGER_H
#define GENERIC_SIGNAL_MANAGER_H

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

    struct ObjectHub{
        QString shared_signal;
        HList<QObject> objs;
    };

public slots:
    void publishGenericSignal(const QString& signal, QObject *publisher);
    void publishGenericDataSignal(const QString& signal, QObject *publisher);
    void unpublishGenericSignal(const QString& signal, QObject *publisher);
    void unpublishGenericDataSignal(const QString& signal, QObject *publisher);

    void subscribeToGenericSignal(const QString& signal, QObject *subscriber);
    void subscribeToGenericDataSignal(const QString& signal, QObject *subscriber);
    void unsubscribeToGenericSignal(const QString& signal, QObject *subscriber);
    void unsubscribeToGenericDataSignal(const QString& signal, QObject *subscriber);

protected:    
    int findObjectAtSignalName(
        QObject *object,
        const QString& signal,
        HList<ObjectHub> *cluster_list);

private:
    HList<ObjectHub> m_signal_publishers;
    HList<ObjectHub> m_signal_subscribers;
    HList<ObjectHub> m_data_signal_publishers;
    HList<ObjectHub> m_data_signal_subscribers;
};

#endif // GENERIC_SIGNAL_MANAGER_H
