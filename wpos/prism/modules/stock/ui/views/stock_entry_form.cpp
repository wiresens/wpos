// file      :  stock_entry_form.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "stock_entry_form.h"
#include "ui_stock_entry_form.h"

StockEntryForm::StockEntryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockEntryForm)
{
    ui->setupUi(this);
    setWindowTitle("Entrée de Stock");
}

StockEntryForm::~StockEntryForm()
{
    delete ui;
}
