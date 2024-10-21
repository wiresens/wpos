// file      :  application_menu.cpp
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_abstract_menu_tree.h"
#include "abstract_menu_tree.h"

AbstractMenuTree::AbstractMenuTree(QWidget *parent)
    : QTreeWidget{parent}, ui{new Ui::AbstractMenuTree}
{
    ui->setupUi(this);
}

