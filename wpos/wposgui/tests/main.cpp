// file      :	main.cpp
// birth     :  06/02/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


/**
 *  Test driver for wposwidget library. This driver aims to exercise all of the
 *  functionality of libwposwidget.so in a clear and consistent way so that
 *  the classes's interface can be easily inferred.
 */

#include <QApplication>
#include <QDebug>
#include <wposwidget/greeterdialog.h>

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
//    GreeterDialog greeter(MD5CryptoHasher{});
//    auto cred = greeter.readCredentials();
//    qDebug() << cred.login << cred.password ;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

//    return app.exec();
}
