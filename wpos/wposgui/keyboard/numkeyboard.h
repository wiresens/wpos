/***************************************************************************
                          numkeyboard.h  -  description
                             -------------------
    begin                : lun jun 30 2003
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

#ifndef NUM_KEYBOARD_H
#define NUM_KEYBOARD_H

#include "numpad.h"

class QKeyEvent;

class NumKeyboard : public NumPad{
    Q_OBJECT
public:
    explicit  NumKeyboard(QWidget *parent);

    int value(){ return m_value;}

public slots:
    //if the num == -1, the CE button has been pressed
    //( use negative numbers for special buttons)
    void reeadKey(int digit)  override;
    void clear() override;

signals:
    void valueChanged(int);

private:
    int m_value{};
};
#endif // NUM_KEYBOARD_H
