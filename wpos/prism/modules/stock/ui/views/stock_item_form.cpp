// file      :  stock_item_form.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "stock_item_form.h"
#include "ui_stock_item_form.h"

StockItemForm::StockItemForm(QWidget *parent) :
    QWidget(parent),
    ui{new Ui::StockItemForm}
{
    ui->setupUi(this);
    ui->caption->clearCaption();
    ui->info->setHidden(true);

    auto date = ui->expiryDate->date();
    auto expiry = QDate(date.year(), date.month(), 1);
    ui->expiryDate->setDate(expiry);

//    ui->cmd->setFrameShape(QFrame::NoFrame);
}

StockItemForm::~StockItemForm()
{
    delete ui;
}
