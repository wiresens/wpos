// file      :	product_test.cpp
// birth     :  06/02/2024
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "product.h"
#include "producttemplate.h"
#include "producttemplate-odb.hxx"
#include "product-odb.hxx"
#include "database.h"
#include "daterange.h"

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>

namespace {
// The fixture for testing ProductTemplate class.
class ProductTemplateTest : public ::testing::Test {
protected:
    odb::database& db{database::instance()};
};

class ProductTest : public ::testing::Test {
protected:
    odb::database& db{database::instance()};
};

using namespace wpos::model;

TEST_F(ProductTemplateTest, ConstructorQueryPersist)
{
    using query = odb::query<Uom>;
    odb::transaction trans(db.begin());
        auto unit = db.query_one<Uom>( query::name == "unit");
        ProductTemplate nescafe_tpl{unit, "Nescafe","6181002002026"};
        ProductTemplate supermont_tpl{unit, "Supermont","85314800300"};
        ProductTemplate fresco_tpl{unit, "Extra Fresco","60096615406"};

        EXPECT_NO_THROW({
            try{
                db.persist(nescafe_tpl);
                db.persist(supermont_tpl);
                db.persist(fresco_tpl);
            }
            catch(std::exception& excep){
                std::cout << excep.what();
            }
        });
    trans.commit();
}

TEST_F(ProductTest, ConstructorName)
{
    {
        using query = odb::query<ProductTemplate>;

        odb::transaction trans(db.begin());
            auto nescafe_tpl = db.query_one<ProductTemplate>(
                query::code == "6181002002026" ||
                query::name == "Nescafe"
            );

            auto supermont_tpl = db.query_one<ProductTemplate>(
                query::code == "85314800300" ||
                query::name == "Supermont"
            );

            Product nescafe {nescafe_tpl, "Nescafe Classic"};
            Product supermont {supermont_tpl, "Supermont 1.5L"};

            EXPECT_EQ( nescafe.name() , "Nescafe Classic");
            EXPECT_EQ( supermont.name() , "Supermont 1.5L");

            EXPECT_NE( nescafe.tmplate().code() , supermont.tmplate().code());

            EXPECT_EQ( nescafe.tmplate().code() , nescafe_tpl->code());
            EXPECT_EQ( supermont.tmplate().code() , supermont_tpl->code());

            EXPECT_ANY_THROW({
                db.persist(supermont);
                db.persist(nescafe);
            });
        trans.commit();
    }
}

TEST_F(ProductTest, ConstructorPrice)
{
    {
        using query = odb::query<ProductTemplate>;
        odb::transaction trans(db.begin());
            auto fresco_tpl = db.query_one<ProductTemplate>(
                query::code == "60096615406" ||
                query::name == "Extra Fresco"
            );

            Product fresco1 {fresco_tpl, "Extra Fresco Orange 300ml", 350};
            Product fresco2 {fresco_tpl, "Extra Fresco Mango 300ml", 350};
            Product fresco3 {fresco_tpl, "Extra Fresco Guava 300ml", 350};

            EXPECT_EQ( fresco1.name() , "Extra Fresco Orange 300ml");
            EXPECT_EQ( fresco2.name() , "Extra Fresco Mango 300ml");
            EXPECT_EQ( fresco3.name() , "Extra Fresco Guava 300ml");

            EXPECT_ANY_THROW({
                db.persist(fresco1);
                db.persist(fresco2);
                db.persist(fresco3);
            });
        trans.commit();
    }
}

TEST_F(ProductTest, SetPrice)
{
    {
        using query = odb::query<Product>;
        odb::transaction trans(db.begin());
            auto supermont = db.query_one<Product>(
                query::name == "Supermont 1.5L"
            );

            supermont->setPrice(400);
            EXPECT_NO_THROW({
                db.update(supermont);
            });
        trans.commit();
    }
}

TEST_F(ProductTest, SetPriceSecond)
{
    {
        using query = odb::query<Product>;
        odb::transaction trans(db.begin());
        auto supermont = db.query_one<Product>(
            query::name == "Supermont 1.5L"
        );

        supermont->setPrice(450);
        EXPECT_NO_THROW({
            db.update(supermont);
        });
        trans.commit();
    }
}

TEST_F(ProductTest, PriceAsOf)
{
    {
        using query = odb::query<Product>;
        odb::transaction trans(db.begin());
        auto supermont = db.query_one<Product>(
            query::name == "Supermont 1.5L"
        );

        EXPECT_EQ(supermont->priceAsOf(wpos::nowLocal()), 450);
        trans.commit();
    }
}

}
