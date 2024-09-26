// file      :  product_creation_navigator.h
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCT_CREATION_NAVIGATOR_H
#define PRODUCT_CREATION_NAVIGATOR_H

#include <wposgui/views/page_navigator.h>

class ProductCreationNavigator : public PageNavigator
{
public:
    ProductCreationNavigator(QWidget *parent = nullptr);

protected:
    virtual QWidget* createPage() override;
};

#endif // PRODUCT_CREATION_NAVIGATOR_H
