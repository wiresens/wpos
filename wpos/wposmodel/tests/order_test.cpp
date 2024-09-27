// file      :	order_test.cpp
// birth     :  11/24/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include <product/product.h>
#include <order/order.h>
#include <core/user.h>
#include <product/product-odb.hxx>
#include <order/order-odb.hxx>
#include <core/user-odb.hxx>
#include <core/session_manager.h>

#include <database.h>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>

namespace {
// The fixture for testing Order class.
class OrderTest : public ::testing::Test {
protected:
    odb::database& db{ wpos::database::instance()};
};

using namespace wpos::model;

TEST_F(OrderTest, ConstructorPersist)
{
    using query = odb::query<Product>;
    wpos::SessionManager sm;

    odb::transaction trans(db.begin());
    UserPtr user = sm.authenticate("gilles", "D0rc4566$");

    OrderPtr order = std::make_shared<Order>(user->employeePtr());
    EXPECT_EQ( order->saler().name() , "BENE POUGOUE Gilles");

    auto fresco = db.query_one<Product>( query::name == "Extra Fresco Orange 300ml");
    auto supermont = db.query_one<Product>( query::name == "Supermont 1.5L");

    order->addItem(fresco, 5);
    EXPECT_EQ( order->total() , 5*fresco->price());

    order->addItem(fresco, 3);
    EXPECT_EQ( order->total() , 8*fresco->price());

    order->addItem(supermont, 4);
    EXPECT_EQ( order->total() , 8*fresco->price() + 4*supermont->price());

    EXPECT_NO_THROW({
        order->persist();
    });

    order->persist();
    sm.logOut(*user);
    EXPECT_NO_THROW({
        db.update(user);
    });

    trans.commit();
}

}
