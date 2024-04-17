// file      :  command_widget.h
// birth     :  2/16/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef COMMAND_WIDGET_H
#define COMMAND_WIDGET_H

#include <QFrame>

namespace Ui {
class CommandWidget;
}

class QPushButton;

class CommandWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CommandWidget(QWidget *parent = nullptr);

    ~CommandWidget();
    QPushButton* addButton(const QString& text, const QIcon& icon);

protected slots:
    virtual void accept(){}
    virtual void cancel(){}

private:
    Ui::CommandWidget *ui;
};

#endif // COMMAND_WIDGET_H
