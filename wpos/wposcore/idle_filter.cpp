// file      :  idle_filter.cpp
// birth     :  10/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "idle_filter.h"
#include <QDebug>

IdleFilter::IdleFilter( QObject *parent , size_t time_out)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &IdleFilter::idle);
    connect(&m_timer, &QTimer::timeout, &m_timer, &QTimer::stop);
    m_timer.setInterval(1000*time_out);
    m_timer.start();
}

void IdleFilter::setActive(bool active)
{
    if (m_active == active) return;

    m_active = active;
    emit activeChanged(m_active);
    emit m_active ? appActivated() : appIdled();
}

void IdleFilter::idle()
{
    setActive(false);
    qDebug() << "Idle";
}

bool IdleFilter::eventFilter(QObject *obj, QEvent *event)
{
    if( ! m_timer.isActive()){
        setActive(true);
        qDebug() << "Active";
    }
    m_timer.start();
    return QObject::eventFilter(obj, event);
}
