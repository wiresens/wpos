// file      :  product_creation_form.cpp
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_product_creation_form.h"
#include "product_creation_form.h"

ProductCreationForm::ProductCreationForm(QWidget *parent)
    : QWidget{parent}, ui{new Ui::ProductCreationForm}
{
    ui->setupUi(this);
    // ui->gbox_inventory->setHidden(true);
    ui->justificationBox->setHidden(true);
    setWindowTitle("Nouveau Produit");
}

ProductCreationForm::~ProductCreationForm(){
    delete ui;
}
