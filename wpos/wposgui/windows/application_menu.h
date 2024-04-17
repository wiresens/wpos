// file      :  application_menu.h
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef APPLICATION_MENU_H
#define APPLICATION_MENU_H

#include <QTreeWidget>

namespace Ui{
class ApplicationMenu;
}

class ApplicationMenu : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ApplicationMenu(QWidget *parent = nullptr);
    ~ApplicationMenu();

signals:

private:
    Ui::ApplicationMenu* ui{};

};

#endif // APPLICATION_MENU_H
