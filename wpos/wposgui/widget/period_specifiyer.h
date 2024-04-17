// file      :  period_specifiyer.h
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef PERIOD_SPECIFIYER_H
#define PERIOD_SPECIFIYER_H

#include "hpanel.h"
#include <QWidget>

namespace Ui {
class PeriodSpecifiyer;
}

class PeriodSpecifiyer : public HPanel
{
    Q_OBJECT

public:
    explicit PeriodSpecifiyer(QWidget *parent = nullptr);
    ~PeriodSpecifiyer();

private:
    Ui::PeriodSpecifiyer *ui;
};

#endif // PERIOD_SPECIFIYER_H
