// file      :  prism_main_window.h
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef PRISM_MAIN_WINDOW_H
#define PRISM_MAIN_WINDOW_H

#include <wposgui/windows/basic_main_window.h>

class PrismMainWindow : public BasicMainWindow
{
    Q_OBJECT
public:
    explicit PrismMainWindow(QWidget *parent = nullptr);

signals:

protected:
    virtual void installCredentialManager() override;
    virtual ApplicationMenu* createMenu(ApplicationDisplay* display) override;

private:
    ApplicationMenu* menu{};
};

#endif // PRISM_MAIN_WINDOW_H
