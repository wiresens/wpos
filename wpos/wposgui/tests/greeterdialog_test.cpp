// file      :	order_test.cpp
// birth     :  06/02/2024
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include <QApplication>
#include <wposwidget/greeterdialog.h>
#include <wposwidget/toolkit.h>

//#include "wposmodel/src/order/order.h"
#include "wposmodel/src/core/user.h"
//#include "order-odb.hxx"
//#include "user-odb.hxx"
#include <wposmodel/include/database.h>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>

namespace {
// The fixture for testing Order class.
class GreeterDialogTest : public ::testing::Test {

protected:
    odb::database& db{::database::instance()};
};

using namespace wpos::model;

TEST_F(GreeterDialogTest, Constructor)
{
    GreeterDialog* greeter = new GreeterDialog( CryptoHasher{});
    auto cred = greeter->readCredentials();
    SessionManager sm;
    odb::transaction trans(db.begin());
        UserPtr gilles = sm.authenticate("gilles","D0rc4566$");
        UserPtr user = sm.authenticate(cred.login.toStdString(), cred.password.toStdString());
        if( user );
            EXPECT_EQ( user->password(), gilles->password() );

        sm.logOut(*user);
        sm.logOut(*gilles);

    trans.commit();
    qApp->exec();
}

}
