// file      :  main_window.cpp
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_main_window.h"
#include "main_window.h"
#include "widget/menu_bar_widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}
{
    ui->setupUi(this);
//    setMenuWidget(new MenuBarWidget(this));
}
