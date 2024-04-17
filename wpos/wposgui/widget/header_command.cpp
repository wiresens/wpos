// file      :  header_command.cpp
// birth     :  03/04/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_header_command.h"
#include "header_command.h"

HeaderCommand::HeaderCommand(
    QWidget *parent) :
    QFrame(parent),
    ui{new Ui::HeaderCommand}
{
    ui->setupUi(this);
    ui->addBtn->setFocus();
    setFrameShape(QFrame::NoFrame);

    connect(ui->addBtn, &QPushButton::clicked, this, &HeaderCommand::add);
    connect(ui->removeBtn, &QPushButton::clicked, this, &HeaderCommand::remove);
}

HeaderCommand::~HeaderCommand(){ delete ui;}

QPushButton* HeaderCommand::addButton(
    const QString& text,
    const QIcon& icon)
{
    auto button = new QPushButton(icon, text, this);
    ui->buttonLayout->addWidget(button);
    return button;
}
