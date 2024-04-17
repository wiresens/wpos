// file      :  main_window.cpp
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_basic_main_window.h"
#include "basic_main_window.h"
#include "application_menu.h"
#include "application_display.h"

#include "widget/menu_bar_widget.h"

#include <QPushButton>

BasicMainWindow::BasicMainWindow(QWidget *parent)
    : QMainWindow{parent}, ui{new Ui::BasicMainWindow}
{
    ui->setupUi(this);
    setMenuWidget(new MenuBarWidget(this));
    setAttribute(Qt::WA_QuitOnClose, true);
    statusBar()->setVisible(true);

   setStyleSheet("QMainWindow::separator { background: yellow;  width: 10px; height: 10px;} \
                  QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
                                         stop:0 #616161, stop: 0.5 #505050, \
                                         stop: 0.6 #434343, stop:1 #656565); \
                                         color: white; \
                                         padding-left: 4px;\
                                         border: 1px solid #6c6c6c;}  \
                   QHeaderView::down-arrow { image: url(:/main/down_arrow);} \
                   QHeaderView::up-arrow { image: url(:/main/up_arrow);} \
                   QLineEdit { border: 1px solid gray; \
                               // border-radius: 6px; \
                               selection-background-color: #aec8ae;} \
                   QLineEdit:disabled { background-color: #eff0f1}");
}

BasicMainWindow::~BasicMainWindow(){
    delete ui;
}

void BasicMainWindow::setupWindow(){
    static bool is_set_up{false};
    if( ! is_set_up){
        installCredentialManager();
        is_set_up = true;
    }
}

void BasicMainWindow::polulate(){
    static bool populated{false};
    if( !populated){
        ApplicationDisplay* display = createDisplay();
        ApplicationMenu* menu = createMenu(display);
        replaceMenu(menu);
        replaceDisplay(display);
        menu->expandAll();
        connect(menu, &ApplicationMenu::itemClicked, display, &ApplicationDisplay::setCurrentPage);
        populated = true;
    }
}

void BasicMainWindow::showMenu(bool checked){
    ui->menu->setVisible(checked);
}

ApplicationMenu* BasicMainWindow::createMenu(ApplicationDisplay*){
    return ui->menu;
}

ApplicationDisplay* BasicMainWindow::createDisplay(){
    return ui->display;
}

void BasicMainWindow::replaceMenu(ApplicationMenu *menu){
    auto wdgt = ui->splitter->replaceWidget(WidgetIndex::Menu, menu);
    if( wdgt){
        menu->setVisible(true);
        wdgt->deleteLater();
        ui->menu = menu;
    }
}

void BasicMainWindow::replaceDisplay(ApplicationDisplay *display){
    auto wdgt = ui->splitter->replaceWidget(WidgetIndex::Display, display);
    if( wdgt){
        display->setVisible(true);
        wdgt->deleteLater();
        ui->display = display;
    }
}
