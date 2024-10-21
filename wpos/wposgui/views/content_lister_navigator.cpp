// file      :  product_listing_navigator.cpp
// birth     :  3/4/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "content_lister_navigator.h"
#include "widget/listing_form.h"

ContentListerNavigator::ContentListerNavigator(
    ListingForm* firstPage,
    QWidget *parent
)
    :PageNavigator{parent},
    tmplate{firstPage}
{
    addPagePrivate(firstPage);
}

QWidget* ContentListerNavigator::createPage(){
    auto page = tmplate->clone(this);
    return page;
}
