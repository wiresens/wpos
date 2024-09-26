// file      :  prism_application.cpp
// birth     :  2/19/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "prism_application.h"
#include "prism_main_window.h"

PrismApplication::PrismApplication(
    int &argc,
    char **argv):
    BasicApplication(argc,argv)
{}

BasicMainWindow& PrismApplication::create(){
    if (! winImpl) winImpl = new PrismMainWindow;
    return *winImpl;
}
