// file      :  menu_bar_widget.cpp
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "menu_bar_widget.h"
#include "ui_menu_bar_widget.h"
#include "windows/basic_main_window.h"
#include <QMenu>

MenuBarWidget::MenuBarWidget(BasicMainWindow *parent) :
    QWidget(parent),
    ui(new Ui::MenuBarWidget)
{
    ui->setupUi(this);
    connect(ui->showMenuBtn, &QPushButton::clicked, parent, &BasicMainWindow::showMenu);
}

MenuBarWidget::~MenuBarWidget()
{
    delete ui;
}
