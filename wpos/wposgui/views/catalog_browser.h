// file      :  catalog_browser.h
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef CATALOG_BROWSER_H
#define CATALOG_BROWSER_H

#include <QWidget>

namespace Ui {
class CatalogBrowser;
}

class ContextMenuBar;
class ModeSelector;
class PageNavigator;
class QAbstractButton;

class CatalogBrowser: public QWidget{
    Q_OBJECT

public:    
    explicit CatalogBrowser(QWidget *parent = nullptr);
    ~CatalogBrowser();

    void addTreeView(PageNavigator* view);
    void addFormView(PageNavigator* view);
    void addKanbanView(PageNavigator* view);
    void addGraphView(PageNavigator* view);

//future features
// private slots:
//     void nextPage(PageNavigator* state);
//     void previousPage(PageNavigator* state);
//     void firstPage(PageNavigator* state);
//     void lastPage(PageNavigator* state);

public slots:
    void switchMode(QAbstractButton *);

protected:
    void setContextMenuBar(ContextMenuBar* cmb);
    void setModeSelector(ModeSelector* selector);

private:
    void addView(PageNavigator* view, QAbstractButton* button);

private:
    Ui::CatalogBrowser *ui{};
    QMap<QAbstractButton*, PageNavigator*> m_views;
    // PageNavigator* state{};
    ModeSelector* m_mode_selector{};
};

#endif // CATALOG_BROWSER_H
