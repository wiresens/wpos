// file      :  navigation_buttons.cpp
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_navigation_buttons.h"
#include "navigation_buttons.h"

NavigationButtons::NavigationButtons(QWidget *parent)
    : QFrame{parent}, ui{new Ui::NavigationButtons}
{
    ui->setupUi(this);
    previous = ui->previous;
    next = ui->next;
    setFrameShape(QFrame::NoFrame);
}

NavigationButtons::~NavigationButtons(){
    delete ui;
}
