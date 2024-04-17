// file      :  basic_main_window.h
// birth     :  2/13/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef BASIC_MAIN_WINDOW_H
#define BASIC_MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui{
class BasicMainWindow;
}

class ApplicationMenu;
class ApplicationDisplay;
class CredentialManager;
class LicenseManager;

class BasicMainWindow : public QMainWindow
{
    Q_OBJECT

    friend class CredentialManager;
public:
    explicit BasicMainWindow(QWidget *parent = nullptr);
    ~BasicMainWindow();

    void setupWindow();
    void polulate();

public slots:
    void showMenu(bool checked = false);

protected:
    virtual void installCredentialManager(){}  // Override this to activate user login
    virtual void removeCredentialManager(){}
    virtual ApplicationMenu* createMenu(ApplicationDisplay*);
    virtual ApplicationDisplay* createDisplay();

private:
    void replaceMenu(ApplicationMenu*);
    void replaceDisplay(ApplicationDisplay*);

private:
    Ui::BasicMainWindow* ui{};
    bool is_set_up{false};

    enum WidgetIndex{
        Menu = 0,
        Display
    };
};

#endif // BASIC_MAIN_WINDOW_H
