// file      :  content_lister.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef CONTENT_LISTER_H
#define CONTENT_LISTER_H

#include <QGroupBox>

namespace Ui {
class ContentLister;
}

class ContentLister : public QGroupBox
{
    Q_OBJECT

public:
    explicit ContentLister(QWidget *parent = nullptr);
    ~ContentLister();

private:
    Ui::ContentLister *ui;
};

#endif // CONTENT_LISTER_H
