/***************************************************************************
                          spkeyboard.h  -  description
                             -------------------
    begin                : dom feb 8 2004
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPKEYBOARD_H
#define SPKEYBOARD_H


#include <QObject>
#include <QtWidgets/QWidget>
#include <wposcore/hlist.h>

class QSignalMapper;
class QPushButton;

namespace Ui {
class  SpKeyboardWidget;
}

class SpKeyboard : public QWidget{
    Q_OBJECT
public:
    explicit  SpKeyboard(QWidget *parent);
    ~SpKeyboard();

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void enterPressed(const QString &text);

public slots:
    void sendEvents(bool on);
    void setExitWithEnter(bool on);
    void clearSlot();

protected slots:
    void keyClicked(int);
    void exitSlot();

private:
    void build(
        QSignalMapper* sgmpr,
        HList<QPushButton> &keys);

private:
    Ui::SpKeyboardWidget    *ui{};
    QObject                 *m_obj_parent{};
    HList<QPushButton>      m_keys;

    bool                    m_caps_lock {false};
    bool                    m_has_send_events {false};
    bool                    m_exit_with_enter {false};
};

#endif
