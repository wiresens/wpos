// file      :  navigation_buttons.h
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NAVIGATION_BUTTONS_H
#define NAVIGATION_BUTTONS_H

#include <QFrame>

namespace Ui {
class  NavigationButtons;
}

class QPushButton;

class NavigationButtons : public QFrame
{
    Q_OBJECT
public:
    explicit NavigationButtons(QWidget *parent = nullptr);
    ~NavigationButtons();

signals:
public:
    QPushButton *previous;
    QPushButton *next;
private:
    Ui::NavigationButtons* ui;
};

#endif // NAVIGATION_BUTTONS_H
