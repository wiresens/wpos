// file      :  stock_creation_navigator.cpp
// birth     :  03/04/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "stock_creation_navigator.h"
#include "stock_entry_form.h"

StockCreationNavigator::StockCreationNavigator(QWidget *parent)
    :PageNavigator{parent} {}

QWidget* StockCreationNavigator::createPage(){
    static StockEntryForm* page{};
    if( !page ) page = new StockEntryForm(this);
    return page;
}
