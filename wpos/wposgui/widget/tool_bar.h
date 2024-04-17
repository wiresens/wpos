// file      :  tool_bar.h
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include <QFrame>
namespace Ui{
class ToolBar;
}

class ContextMenuBar;
class ModeSelector;

class ToolBar : public QFrame
{
public:
    ToolBar(QWidget* parent=nullptr);
    ~ToolBar();

    void setContextMenuBar(ContextMenuBar* context_menu_bar);
    void setModeSelector(ModeSelector* mode_selector);

private:
    Ui::ToolBar* ui{};
    ContextMenuBar* m_context_menu_bar{};
    ModeSelector* m_mode_selector{};
};

#endif // TOOL_BAR_H
