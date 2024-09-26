// file      :  stock_creation_navigator.h
// birth     :  03/04/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef STOCK_CREATION_NAVIGATOR_H
#define STOCK_CREATION_NAVIGATOR_H

#include <wposgui/views/page_navigator.h>

class StockCreationNavigator : public PageNavigator
{
public:
    StockCreationNavigator(QWidget *parent = nullptr);

protected:
    virtual QWidget* createPage() override;
};

#endif // STOCK_CREATION_NAVIGATOR_H
