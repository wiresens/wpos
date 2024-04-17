// file      :  stock_entry_form.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef STOCK_ENTRY_FORM_H
#define STOCK_ENTRY_FORM_H

#include <QWidget>

namespace Ui {
class StockEntryForm;
}

class StockEntryForm : public QWidget
{
    Q_OBJECT

public:
    explicit StockEntryForm(QWidget *parent = nullptr);
    ~StockEntryForm();

private:
    Ui::StockEntryForm *ui;
};

#endif // STOCK_ENTRY_FORM_H
