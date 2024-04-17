// file      :  tool_bar.cpp
// birth     :  02/28/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "ui_tool_bar.h"
#include "tool_bar.h"
#include "mode_selector.h"
#include "context_menu_bar.h"

ToolBar::ToolBar(QWidget *parent):
    QFrame(parent), ui{new Ui::ToolBar}
{
    ui->setupUi(this);
}

ToolBar::~ToolBar(){ delete ui; }

void ToolBar::setContextMenuBar(ContextMenuBar* context_menu_bar){
    if(m_context_menu_bar){
        ui->menuLayout->removeWidget(m_context_menu_bar);
        m_context_menu_bar->setParent(nullptr);
    }
    ui->menuLayout->addWidget(context_menu_bar);
    m_context_menu_bar = context_menu_bar;
}

void ToolBar::setModeSelector(ModeSelector* mode_selector){
    if (m_mode_selector) {
        ui->viewLayout->removeWidget(m_mode_selector);
        m_mode_selector->setParent(nullptr);
    }
    ui->viewLayout->addWidget(mode_selector);
    m_mode_selector = mode_selector;
}
