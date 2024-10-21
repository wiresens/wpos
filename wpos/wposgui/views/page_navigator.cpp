// file      :  catalog_navigator.cpp
// birth     :  12/17/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_page_navigator.h"
#include "page_navigator.h"

#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>

PageNavigator::PageNavigator(QWidget *parent)
    :QWidget{parent},
    ui{new Ui::PageNavigator}
{
    ui->setupUi(this);

    //We first hide th context menu bar
    ui->gridLayout->removeWidget(ui->contextBar);
    ui->contextBar->setParent(nullptr);

    connect(ui->navigationButtons->previous, &QPushButton::clicked,
            this, &PageNavigator::previousPage);
    connect(ui->navigationButtons->next, &QPushButton::clicked,
            this, &PageNavigator::nextPage);
}

// PageNavigator::PageNavigator(
//     QAbstractItemModel &model,
//     QWidget *parent)
//     :QWidget(parent),
//     model{model},
//     ui{new Ui::PageNavigator}
// {
//     ui->setupUi(this);
//     toolBar   = new QToolBar(ui->toolbarFrame);
//     statusBar = new QStatusBar(ui->navigationFrame);

//     connect(ui->navigationButtons->previous, &QPushButton::clicked, this, &PageNavigator::previousPage);
//     connect(ui->navigationButtons->next, &QPushButton::clicked, this, &PageNavigator::nextPage);
// }

PageNavigator::~PageNavigator(){
    if( ! ui->contextBar->parent()) ui->contextBar->deleteLater();
    delete ui;
}

void PageNavigator::addPage(){
    addPagePrivate(createPage());
}

void PageNavigator::addPage(QWidget* page){
    addPagePrivate(page);
}

ContextMenuBar* PageNavigator::contextMenuBar(){
    return ui->contextBar;
}

void PageNavigator::nextPage(){
    auto pos = ui->stack->currentIndex();
    auto last = ui->stack->count() - 1;

    if( pos < last)  ui->stack->setCurrentIndex( pos + 1);
    if( pos == 0) ui->navigationButtons->previous->setEnabled(true);
    if( pos + 1 == last) ui->navigationButtons->next->setEnabled(false);
}

void PageNavigator::previousPage(){
    auto pos = ui->stack->currentIndex();
    auto last = ui->stack->count() - 1;

    if( pos > 0 ) ui->stack->setCurrentIndex( pos - 1);
    if( pos == last ) ui->navigationButtons->next->setEnabled(true);
    if( pos - 1 == 0) ui->navigationButtons->previous->setEnabled(false);
}

void PageNavigator::firstPage(){
    ui->stack->setCurrentIndex(0);
}

void PageNavigator::lastPage(){
   ui->stack->setCurrentIndex(ui->stack->count() - 1);
}

void PageNavigator::removePage(QWidget* page){
    ui->stack->removeWidget(page);
}

void PageNavigator::setCurrentPage(QWidget* page){
    ui->stack->setCurrentWidget(page);
}

QWidget* PageNavigator::currentPage() const{
    return ui->stack->currentWidget();
}

void PageNavigator::addPagePrivate(QWidget* page){
    ui->stack->addWidget(page);
}
