/***************************************************************************
                          basickeyboardbox.h  -  description
                             -------------------
    begin                :  Aug 20 2003
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

#ifndef NUMPAD_H
#define NUMPAD_H

#include <QWidget>
#include <QLCDNumber>

namespace Ui {
class NumPad;
} // namespace Ui

class QSignalMapper;

class NumPad :
    public QWidget
{
    Q_OBJECT
public:
    explicit NumPad(QWidget* parent = nullptr);
    ~NumPad(){ delete ui; }

    void showDisplay();
    void hideDisplay();

    void setDirty(bool dirty = true){ m_is_dirty = dirty;}

public slots:
    //if the num == -1 then the CE button has been pressed
    //( use negative numbers for special buttons)
    virtual void clear();
    virtual void reeadKey(int) = 0;

protected:
    void enterEvent(QEvent*){setFocus();}
    void leaveEvent(QEvent*){clearFocus();}
    void keyPressEvent(QKeyEvent *event);

    bool isDirty() const { return m_is_dirty; }

    void display(double value);
    void setSegmentStyle(QLCDNumber::SegmentStyle style);
    void setDigitCount(unsigned count);

    void cefontIsDotfont();
    void dotInit( const QString& value);
    void dotVisible(bool visibility);

    void removeKey(QWidget* key);
    void addKey(QWidget* key, int row, int column, int rowSpan, int columnSpan);

    int ce() const;
    int dot() const;
    QWidget *dotKey() const;
    QWidget *ceKey() const;

private:
    Ui::NumPad* ui{};
    QSignalMapper* signalMapper{};
    bool m_is_dirty{false};
};

#endif // NUMPAD_H
