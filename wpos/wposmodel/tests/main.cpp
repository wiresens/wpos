// file      :	main.cpp
// birth     :  11/20/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


/**
 *  Test driver for wposmodel library. This driver aims to exercise all of the
 *  functionality of libwposmodel.so in a clear and consistent way so that
 *  the classes's interface can be easily inferred.
 */

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
