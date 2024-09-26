// file      :  product_creation_navigator.cpp
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "product_creation_navigator.h"
#include "product_creation_form.h"

ProductCreationNavigator::ProductCreationNavigator(QWidget *parent)
    :PageNavigator{parent} {}

QWidget* ProductCreationNavigator::createPage(){
    static ProductCreationForm* page{};
    if( !page ) page = new ProductCreationForm(this);
    return page;
}

