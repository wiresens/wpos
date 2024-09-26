// file      :  stock_item_form.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef STOCK_ITEM_FORM_H
#define STOCK_ITEM_FORM_H

#include <QWidget>

namespace Ui {
class StockItemForm;
}

class StockItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit StockItemForm(QWidget *parent = nullptr);
    ~StockItemForm();

private:
    Ui::StockItemForm *ui;
};

#endif // STOCK_ITEM_FORM_H
