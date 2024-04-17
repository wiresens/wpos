// file      :  listing_form.cpp
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "listing_form.h"
#include "ui_listing_form.h"

ListingForm::ListingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListingForm)
{
    ui->setupUi(this);
    // ui->captionFrame->setVisible(false);
}

ListingForm::~ListingForm()
{
    delete ui;
}

ListingForm* ListingForm::clone(QWidget *parent){
    return new ListingForm(parent);
}
