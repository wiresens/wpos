// file      :  application_display.cpp
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_application_display.h"
#include "application_display.h"
#include "widget/menu_bar_widget.h"

#include <QDebug>
#include <QTreeWidgetItem>

ApplicationDisplay::ApplicationDisplay(QWidget *parent)
    : QStackedWidget{parent}, ui{new Ui::ApplicationDisplay}
{
    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, ui->tabWidget, &QTabWidget::removeTab);
}

ApplicationDisplay::~ApplicationDisplay(){
    delete ui;
}

void ApplicationDisplay::addPage(QTreeWidgetItem* item, QWidget* page){
    pages[item] = page;
}

void ApplicationDisplay::setCurrentPage(QTreeWidgetItem* item){
    auto page = pages[item];
    if( ui->tabWidget->indexOf(page) < 0)
        ui->tabWidget->addTab(page, item->icon(0) , item->text(0));
    ui->tabWidget->setCurrentWidget(page);
    setCurrentWidget(ui->tabWidget);
}
