// file      :  product_creation_form.h
// birth     :  2/28/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCT_CREATION_FORM_H
#define PRODUCT_CREATION_FORM_H

#include <QWidget>

namespace Ui {
class ProductCreationForm;
}

class ProductCreationForm : public QWidget
{
    Q_OBJECT
public:
    explicit ProductCreationForm(QWidget *parent = nullptr);
    ~ProductCreationForm();

private:
    Ui::ProductCreationForm* ui{};

signals:
};

#endif // PRODUCT_CREATION_FORM_H
