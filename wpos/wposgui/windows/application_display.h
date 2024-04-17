// file      :  application_display.h
// birth     :  2/17/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef APPLICATION_DISPLAY_H
#define APPLICATION_DISPLAY_H

#include <QStackedWidget>
#include <QMenu>

namespace Ui{
class ApplicationDisplay;
}

class QTreeWidgetItem;

class ApplicationDisplay : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ApplicationDisplay(QWidget *parent = nullptr);
    ~ApplicationDisplay();

    void addPage(QTreeWidgetItem *item, QWidget* page);

public slots:
    void setCurrentPage(QTreeWidgetItem* item);

private:
    QMap<QTreeWidgetItem*, QWidget*> pages;
    Ui::ApplicationDisplay* ui{};

};

#endif // APPLICATION_DISPLAY_H
