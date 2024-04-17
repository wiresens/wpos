// file      :  mode_selector.h
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef MODE_SELECTOR_H
#define MODE_SELECTOR_H

#include <QFrame>
namespace Ui{
class ModeSelector;
}

class QButtonGroup;
class QAbstractButton;
class CatalogBrowser;

class ModeSelector : public QFrame{
    Q_OBJECT

public:
    explicit ModeSelector(CatalogBrowser* parent=nullptr);
    ~ModeSelector();

    QAbstractButton* treeButton() const;
    QAbstractButton* formButton() const;
    QAbstractButton* kanbanButton() const;
    QAbstractButton* graphButton() const;

private:
    Ui::ModeSelector* ui;
    QButtonGroup* m_button_group;
};

#endif // MODE_SELECTOR_H
