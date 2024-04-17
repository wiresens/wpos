// file      :  menu_bar_widget.h
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef MENU_BAR_WIDGET_H
#define MENU_BAR_WIDGET_H

#include <QWidget>

namespace Ui {
class MenuBarWidget;
}

class BasicMainWindow;

class MenuBarWidget : public QWidget
{
    Q_OBJECT


public:
    explicit MenuBarWidget(BasicMainWindow *parent = nullptr);
    ~MenuBarWidget();

private:
    Ui::MenuBarWidget *ui;
};

#endif // MENU_BAR_WIDGET_H
