// file      :  context_menu_bar.h
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef CONTEXT_MENU_BAR_H
#define CONTEXT_MENU_BAR_H

#include <QFrame>
namespace Ui{
class ContextMenuBar;
}

class QButtonGroup;
class QPushButton;

class ContextMenuBar : public QFrame
{
public:
    explicit ContextMenuBar(QWidget* parent=nullptr);
    ~ContextMenuBar();

    QPushButton* addButton(const QString& text, const QIcon& icon);

private:
    Ui::ContextMenuBar* ui;
};

#endif // CONTEXT_MENU_BAR_H
