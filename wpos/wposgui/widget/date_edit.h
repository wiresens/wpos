// file      :  date_edit.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef DATE_EDIT_H
#define DATE_EDIT_H

#include <QDateEdit>

namespace Ui {
class DateEdit;
}

class DateEdit : public QDateEdit
{
    Q_OBJECT

public:
    explicit DateEdit(QWidget *parent = nullptr);
    ~DateEdit();

private:
    Ui::DateEdit *ui;
};

#endif // DATE_EDIT_H
