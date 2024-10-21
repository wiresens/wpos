// file      :  catalog_browser.cpp
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_catalog_browser.h"
#include "catalog_browser.h"
#include "page_navigator.h"
#include "widget/mode_selector.h"
#include <QAbstractButton>

CatalogBrowser::CatalogBrowser(QWidget *parent)
    :QWidget{parent},
    ui{new Ui::CatalogBrowser}
{
    ui->setupUi(this);
    m_mode_selector = new ModeSelector(this);
    ui->toolbar->setModeSelector(m_mode_selector);
}

CatalogBrowser::~CatalogBrowser(){
    delete ui;
    if( ! m_mode_selector->parent()) m_mode_selector->deleteLater();
}

void CatalogBrowser::addTreeView(PageNavigator* view){
    addView(view, m_mode_selector->treeButton());
}

void CatalogBrowser::addFormView(PageNavigator *view){
    addView(view, m_mode_selector->formButton());
}

void CatalogBrowser::addKanbanView(PageNavigator *view){
    addView(view, m_mode_selector->kanbanButton());
}

void CatalogBrowser::addGraphView(PageNavigator *view){
    addView(view, m_mode_selector->graphButton());
}

// void CatalogBrowser::nextPage(PageNavigator* state){
//     state->nextPage();
// }

// void CatalogBrowser::previousPage(PageNavigator* state){
//     state->previousPage();
// }

// void CatalogBrowser::firstPage(PageNavigator* state){
//     state->firstPage();
// }

// void CatalogBrowser::lastPage(PageNavigator* state){
//     state->lastPage();
// }

void CatalogBrowser::switchMode(QAbstractButton* button){
    PageNavigator* navigator = m_views[button];
    ui->stack->setCurrentWidget(navigator);
    setContextMenuBar(navigator->contextMenuBar());
}

void CatalogBrowser::setContextMenuBar(ContextMenuBar* cmb){
    ui->toolbar->setContextMenuBar(cmb);
}

void CatalogBrowser::addView(PageNavigator* view, QAbstractButton* button){
    ui->stack->addWidget(view);
    m_views[button] = view;
    button->setVisible(true);
}
