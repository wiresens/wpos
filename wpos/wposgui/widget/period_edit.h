// file      :  period_edit.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef PERIOD_EDIT_H
#define PERIOD_EDIT_H

#include <QWidget>

namespace Ui {
class PeriodEdit;
}

class PeriodEdit : public QWidget
{
    Q_OBJECT

public:
    explicit PeriodEdit(QWidget *parent = nullptr);
    ~PeriodEdit();

    QDate startDate() const;
    QDate endDate() const;

private slots:
    void updateEndRage();

private:
    Ui::PeriodEdit *ui;
};

#endif // PERIOD_EDIT_H
