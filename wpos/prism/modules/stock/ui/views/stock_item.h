// file      :  stock_item.h
// birth     :  2/29/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef STOCKITEM_H
#define STOCKITEM_H

#include <QWidget>

namespace Ui {
class StockItem;
}

class StockItem : public QWidget
{
    Q_OBJECT
public:
    explicit StockItem(QWidget *parent = nullptr);
    ~StockItem();

signals:
private:
    Ui::StockItem* ui{};
};

#endif // STOCKITEM_H
