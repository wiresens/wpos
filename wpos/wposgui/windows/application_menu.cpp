// file      :  application_menu.cpp
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_application_menu.h"
#include "application_menu.h"

#include <QDebug>

ApplicationMenu::ApplicationMenu(QWidget *parent)
    : QTreeWidget{parent}, ui{new Ui::ApplicationMenu}
{
    ui->setupUi(this);
}

ApplicationMenu::~ApplicationMenu(){
    delete ui;
    qDebug() << "Called ApplicationMenu::~ApplicationMenu()";
}
