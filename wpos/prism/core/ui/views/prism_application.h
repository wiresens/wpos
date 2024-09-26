// file      :  prism_application.h
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef PRISM_APPLICATION_H
#define PRISM_APPLICATION_H

class BasicMainWindow;

#include <wposgui/application/basic_application.h>

class PrismApplication : public BasicApplication
{
    Q_OBJECT
public:
    PrismApplication(int &argc, char **argv);

protected:
    virtual BasicMainWindow& create() override;

private:
    BasicMainWindow* winImpl{};
};

#endif // PRISM_APPLICATION_H
