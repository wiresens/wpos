// file      :  context_menu_bar.cpp
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_context_menu_bar.h"
#include "context_menu_bar.h"

#include <QButtonGroup>

ContextMenuBar::ContextMenuBar(QWidget *parent):
    QFrame(parent), ui{new Ui::ContextMenuBar}
{
    ui->setupUi(this);
}

ContextMenuBar::~ContextMenuBar(){
    delete ui;
}

QPushButton* ContextMenuBar::addButton(
    const QString& text,
    const QIcon& icon)
{
    auto button = new QPushButton(icon,text, this);
    button->setIconSize(QSize(24, 24));
    button->setFlat(true);
    ui->buttonLayout->addWidget(button);
    return button;
}
