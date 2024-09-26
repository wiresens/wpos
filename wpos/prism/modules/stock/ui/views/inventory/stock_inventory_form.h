// file      :  stock_inventory_form.h
// birth     :  03/03/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef STOCK_INVENTORY_FORM_H
#define STOCK_INVENTORY_FORM_H

#include <QWidget>

namespace Ui {
class StockInventoryForm;
}

class StockInventoryForm : public QWidget
{
public:
    StockInventoryForm(QWidget* parent=nullptr);
    ~StockInventoryForm();

private:
    Ui::StockInventoryForm* ui{};
};

#endif // STOCK_INVENTORY_FORM_H
