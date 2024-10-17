// file      :  idle_filter.h
// birth     :  10/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef IDLE_FILTER_H
#define IDLE_FILTER_H

#include <QObject>
#include <QTimer>

class IdleFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
    explicit IdleFilter(
        QObject *parent = nullptr,
        size_t time_out = 10); //10s

    inline bool isActive() const { return m_active; }

public slots:
    void setActive(bool active);

private slots:
    void idle();

signals:
    void activeChanged(bool active);
    void appActivated();
    void appIdled();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    QTimer m_timer;
    bool m_active {true};
};

#endif // IDLE_FILTER_H
