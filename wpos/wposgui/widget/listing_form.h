// file      :  listing_form.h
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef LISTING_FORM_H
#define LISTING_FORM_H

#include <QWidget>

namespace Ui {
class ListingForm;
}

class ListingForm : public QWidget
{
    Q_OBJECT

public:
    explicit ListingForm(QWidget *parent = nullptr);
    ~ListingForm();

    virtual ListingForm* clone(QWidget *parent = nullptr);


private:
    Ui::ListingForm *ui;
};

#endif // LISTING_FORM_H
