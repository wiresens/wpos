// file      :  stock_inventory_form.cpp
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "ui_stock_inventory_form.h"
#include "stock_inventory_form.h"

StockInventoryForm::StockInventoryForm(QWidget *parent)
    :QWidget{parent}, ui{new Ui::StockInventoryForm}
{
    ui->setupUi(this);
    setWindowTitle("Inventaire de Stock");
}

StockInventoryForm::~StockInventoryForm(){
    delete ui;
}
