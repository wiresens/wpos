// file      :  content_lister_navigator.h
// birth     :  3/4/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef CONTENT_LISTER_NAVIGATOR_H
#define CONTENT_LISTER_NAVIGATOR_H

#include "views/page_navigator.h"
class ListingForm;

class ContentListerNavigator : public PageNavigator
{
public:
    explicit ContentListerNavigator(ListingForm* firstPage, QWidget *parent = nullptr);

protected:
    virtual QWidget* createPage() override;

private:
    ListingForm* tmplate;
};

#endif // CONTENT_LISTER_NAVIGATOR_H
