// file      :  hpanel.h
// birth     :  2/14/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef HPANEL_H
#define HPANEL_H

#include <QWidget>

namespace Ui {
class HPanel;
}

class HPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HPanel(QWidget *parent = nullptr);
    ~HPanel();

private:
    Ui::HPanel *ui;
};

#endif // HPANEL_H
