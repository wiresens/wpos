// file      :  basic_application.h
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef BASIC_APPLICATION_H
#define BASIC_APPLICATION_H

#include <QApplication>

class BasicMainWindow;

class BasicApplication : public QApplication
{
    Q_OBJECT
protected:
    BasicApplication(int &argc, char **argv);

public:
    BasicMainWindow& createMainWindow();

protected:
    virtual void loadPlugins(){}
    virtual void loadModules(){}
    virtual void installLicenseManager(){}

    virtual BasicMainWindow& create() = 0;

private:
    void loadSettings();
    void setupApplication(BasicMainWindow&);
};

#endif // BASIC_APPLICATION_H
