/***************************************************************************
                          floatkeyboard.h  -  description
                             -------------------
    begin                : lun jun 30 2003
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

#ifndef FLOAT_KEYBOARD_H
#define FLOAT_KEYBOARD_H

#include "numpad.h"

class QKeyEvent;
class FloatKeyboard : public NumPad {
    Q_OBJECT

public:
    explicit FloatKeyboard(QWidget *parent=nullptr);

    double value(){return value_;}
    void setValue(double value);

public slots:
    //if the num == -1, the CE button has been pressed
    //( use negative numbers for special buttons)
    void receiveDigit(int digit)  override;
    void clear() override;

signals:
    void valueChanged(double);

private:
    double value_{};
    QString numStr;
};

#endif // FLOAT_KEYBOARD_H
