// file      :  header_command.h
// birth     :  03/04/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef HEADER_COMMAND_H
#define HEADER_COMMAND_H

#include <QFrame>

namespace Ui {
class HeaderCommand;
}

class QPushButton;

class HeaderCommand : public QFrame
{
    Q_OBJECT

public:
    explicit HeaderCommand(QWidget *parent = nullptr);

    ~HeaderCommand();
    QPushButton* addButton(const QString& text, const QIcon& icon);

protected slots:
    virtual void add(){}
    virtual void remove(){}

private:
    Ui::HeaderCommand *ui;
};

#endif // HEADER_COMMAND_H
