// file      :  stock_item_command_widget.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef STOCK_ITEM_COMMAND_WIDGET_H
#define STOCK_ITEM_COMMAND_WIDGET_H

#include <wposgui/widget/command_widget.h>

class StockItemCommandWidget : public CommandWidget
{
    Q_OBJECT

public:
    explicit StockItemCommandWidget(QWidget *parent = nullptr);

protected slots:
    virtual void accept() override;
    virtual void cancel() override;
};

#endif // STOCK_ITEM_COMMAND_WIDGET
