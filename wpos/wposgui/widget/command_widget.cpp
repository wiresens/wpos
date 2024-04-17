// file      :  command_widget.cpp
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "command_widget.h"
#include "ui_command_widget.h"

CommandWidget::CommandWidget(
    QWidget *parent) :
    QFrame(parent),
    ui{new Ui::CommandWidget}
{
    ui->setupUi(this);
    ui->acceptBtn->setFocus();
    setFrameShape(QFrame::NoFrame);

    connect(ui->acceptBtn, &QPushButton::clicked, this, &CommandWidget::accept);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &CommandWidget::cancel);
}

CommandWidget::~CommandWidget()
{
    delete ui;
}

QPushButton* CommandWidget::addButton(
    const QString& text,
    const QIcon& icon)
{
    auto button = new QPushButton(icon, text, this);
    ui->buttonLayout->addWidget(button);
    return button;
}
