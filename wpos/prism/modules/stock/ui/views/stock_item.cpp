// file      :  stock_item.cpp
// birth     :  2/29/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "stock_item.h"
#include "ui_stock_item.h"

StockItem::StockItem(QWidget *parent)
    : QWidget{parent}, ui{new Ui::StockItem}
{
    ui->setupUi(this);
}

StockItem::~StockItem(){
    delete ui;
}
