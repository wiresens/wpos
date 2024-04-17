// file      :  period_edit.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "period_edit.h"
#include "ui_period_edit.h"

PeriodEdit::PeriodEdit(QWidget *parent) :
    QWidget(parent),
    ui{new Ui::PeriodEdit}
{
    ui->setupUi(this);
    connect(ui->start, &DateEdit::dateChanged, this, &PeriodEdit::updateEndRage);
}

PeriodEdit::~PeriodEdit()
{
    delete ui;
}

QDate PeriodEdit::startDate() const{
    return ui->start->date();
}

QDate PeriodEdit::endDate() const{
    return ui->end->date();
}

void PeriodEdit::updateEndRage(){
    ui->end->setMinimumDate(ui->start->date());
}
