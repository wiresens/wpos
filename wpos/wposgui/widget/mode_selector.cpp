// file      :  mode_selector.cpp
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_mode_selector.h"
#include "mode_selector.h"
#include "views/catalog_browser.h"

#include <QButtonGroup>

ModeSelector::ModeSelector(CatalogBrowser *parent):
    QFrame(parent), ui{new Ui::ModeSelector}
{
    ui->setupUi(this);
    m_button_group = new QButtonGroup(this);
    m_button_group->addButton(ui->treeView);
    m_button_group->addButton(ui->formView);
    m_button_group->addButton(ui->kanbanView);
    m_button_group->addButton(ui->graphView);
    m_button_group->setExclusive(true);

    ui->treeView->setVisible(false);
    ui->formView->setVisible(false);
    ui->kanbanView->setVisible(false);
    ui->graphView->setVisible(false);

    connect(m_button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            parent, &CatalogBrowser::switchMode);
}

ModeSelector::~ModeSelector(){
    delete ui;
}

QAbstractButton *ModeSelector::treeButton() const{
    return ui->treeView;
}

QAbstractButton *ModeSelector::formButton() const{
    return ui->formView;
}

QAbstractButton *ModeSelector::kanbanButton() const{
    return ui->kanbanView;
}

QAbstractButton *ModeSelector::graphButton() const{
    ui->graphView;
}
