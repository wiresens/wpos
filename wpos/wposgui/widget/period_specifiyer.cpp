// file      :  period_specifiyer.cpp
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "period_specifiyer.h"
#include "ui_period_specifiyer.h"

PeriodSpecifiyer::PeriodSpecifiyer(QWidget *parent) :
    HPanel(parent),
    ui(new Ui::PeriodSpecifiyer)
{
    ui->setupUi(this);
}

PeriodSpecifiyer::~PeriodSpecifiyer()
{
    delete ui;
}
