// file      :  content_lister.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "content_lister.h"
#include "ui_content_lister.h"

ContentLister::ContentLister(QWidget *parent) :
    QGroupBox(parent),
    ui{new Ui::ContentLister}
{
    ui->setupUi(this);
}

ContentLister::~ContentLister()
{
    delete ui;
}
