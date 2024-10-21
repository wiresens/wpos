// file      :  application_menu.h
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ABSTRACT_MENU_TREE_H
#define ABSTRACT_MENU_TREE_H

#include <QTreeWidget>

namespace Ui{
class AbstractMenuTree;
}

class AbstractMenuTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AbstractMenuTree(QWidget *parent = nullptr);

signals:

private:
    Ui::AbstractMenuTree* ui{};

};

#endif // ABSTRACT_MENU_TREE_H
