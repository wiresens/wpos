// file      :  catalog_navigator.h
// birth     :  12/17/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef PAGE_NAVIGATOR_H
#define PAGE_NAVIGATOR_H

#include <QWidget>
#include <QAbstractItemModel>

namespace Ui {
class PageNavigator;
}

class QToolBar;
class QStatusBar;
class ContextMenuBar;

class PageNavigator : public QWidget{
    Q_OBJECT

public:
    // explicit CatalogNavigator(
    //     QAbstractItemModel &model ,
    //     QWidget *parent = nullptr);

    explicit PageNavigator(QWidget *parent = nullptr);

    ~PageNavigator();
    void addPage();
    void addPage(QWidget* page);
    ContextMenuBar* contextMenuBar();

private slots:
    void nextPage();
    void previousPage();
    void firstPage();
    void lastPage();

protected:
    virtual QWidget* createPage() = 0;

    void removePage(QWidget* page);
    void setCurrentPage(QWidget *page);
    QWidget* currentPage() const;
    void addPagePrivate(QWidget* page);

protected slots:
    virtual void open(){}
    virtual void close(){}
    virtual void create(){}
    virtual void remove(){}
    virtual void select(){}
    virtual void update(){}

private:
    // QAbstractItemModel& model;
    Ui::PageNavigator *ui{};

};

#endif // PAGE_NAVIGATOR_H
