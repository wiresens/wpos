// file      :  caption_widget.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef CAPTION_WIDGET_H
#define CAPTION_WIDGET_H

#include <QFrame>

namespace Ui {
class CaptionWidget;
}

class CaptionWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CaptionWidget(
        QWidget *parent = nullptr,
        const QString& caption = QString());

    ~CaptionWidget();

    void clearCaption();
    void setCaption(const QString& caption);
    void clearOther();
    void setOther(const QString& other);
    void clearAll();

private:
    Ui::CaptionWidget *ui;
};

#endif // CAPTION_WIDGET_H
