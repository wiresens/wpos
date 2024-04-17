// file      :  hpanel.cpp
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "hpanel.h"
#include "ui_hpanel.h"

HPanel::HPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPanel)
{
    ui->setupUi(this);
}

HPanel::~HPanel()
{
    delete ui;
}
