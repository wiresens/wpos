// file      :  date_edit.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "date_edit.h"
#include "ui_date_edit.h"

DateEdit::DateEdit(QWidget *parent) :
    QDateEdit(parent),
    ui{new Ui::DateEdit}
{
    ui->setupUi(this);
    setDate(QDate::currentDate());
}

DateEdit::~DateEdit()
{
    delete ui;
}
