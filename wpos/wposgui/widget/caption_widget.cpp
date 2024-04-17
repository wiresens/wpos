// file      :  caption_widget.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "caption_widget.h"
#include "ui_caption_widget.h"

CaptionWidget::CaptionWidget(
    QWidget *parent,
    const QString &caption) :
    QFrame(parent),
    ui{new Ui::CaptionWidget}
{
    ui->setupUi(this);
    setCaption(caption);
}

CaptionWidget::~CaptionWidget()
{
    delete ui;
}


void CaptionWidget::clearCaption(){
    ui->caption->clear();
}

void CaptionWidget::setCaption(const QString& caption){
    ui->caption->setText(caption);
}

void CaptionWidget::clearOther(){
    ui->other->clear();
}

void CaptionWidget::setOther(const QString& other){
    ui->other->setText(other);
}

void CaptionWidget::clearAll(){
    ui->caption->clear();
    ui->other->clear();
}
