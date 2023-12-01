// file      :	uom_test.cpp
// birth     :  11/20/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include "uom.h"
#include "uom-odb.hxx"
#include <database.h>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>
#include <memory>

using namespace wpos::model;

namespace {
// The fixture for testing UomType class.
class UomTypeTest : public ::testing::Test {
protected:
    odb::database& db{database::instance()};

    UomType volume_t{"Volume"};
    UomType weight_t{"Weight"};
    UomType number_t{"Number"};

    void persist(){
        db.persist(volume_t);
        db.persist(weight_t);
        db.persist(number_t);
    }
};

TEST_F(UomTypeTest, Constructor)
{
    EXPECT_NO_THROW({ UomType uom_t("u") ;});

    UomType uom_t("u") ;
    EXPECT_EQ(uom_t.name(), "u");
    EXPECT_NE(uom_t.name(), "ui");
}

TEST_F(UomTypeTest, Name)
{
    EXPECT_NE(volume_t.name(), weight_t.name());
    EXPECT_NE(volume_t.name(), number_t.name());
    EXPECT_NE(number_t.name(), weight_t.name());
}

TEST_F(UomTypeTest, PersistTreeItems)
{
    {
        odb::transaction trans(db.begin());
        EXPECT_NO_THROW({ persist();});
        EXPECT_ANY_THROW({ persist();});
        trans.commit();
    }
}

TEST_F(UomTypeTest, LoadAndPersist)
{
    {
        odb::transaction trans(db.begin());
        auto vol = db.load<UomType>(1);
        EXPECT_TRUE( vol.get() != nullptr );
        EXPECT_NE( vol->id , volume_t.id);

        UomType length_t{"Length"};
        EXPECT_EQ( length_t.id , volume_t.id);

        EXPECT_NO_THROW(db.persist(length_t););
        auto length1 = db.load<UomType>(length_t.id);
        EXPECT_EQ( length1->id , length_t.id);
        EXPECT_ANY_THROW({ db.persist(length_t); });
        trans.commit();
    }
}

// The fixture for testing Uom class.
class UomTest : public ::testing::Test {
protected:
    odb::database& db{database::instance()};

    UomType* uom_type = new UomType{"Volume"};
    Uom uom1 {std::make_shared<UomType>("Volume"), "liter", "l"};
    Uom uom2 {std::shared_ptr<UomType>(uom_type), "cubic meter", "m3"};
};

TEST_F(UomTest, Constructor)
{
    using query = odb::query<UomType>;
    odb::transaction trans(db.begin());
    auto volume_t = db.query_one<UomType>( query::name == "Volume");
    EXPECT_NO_THROW ( Uom uom(volume_t, "galon", "g") ;);
    trans.commit();

}

TEST_F(UomTest, Type)
{
    EXPECT_EQ(uom_type->name(), "Volume");
    EXPECT_NE(uom_type->name(), "Weight");
    EXPECT_EQ(uom1.type(), uom2.type());
}

TEST_F(UomTest, Name)
{
    EXPECT_EQ(uom1.name(), "liter");
    EXPECT_EQ(uom2.name(), "cubic meter");
}

TEST_F(UomTest, QueryAndPersist)
{
    {
        using query = odb::query<UomType>;
        using result = odb::result<UomType>;

        odb::transaction trans(db.begin());

        auto volume_t = db.query_one<UomType>( query::name == "Volume");
        auto number_t = db.query_one<UomType>( query::name == "Number");

        Uom liter {volume_t, "liter", "l"};
        Uom cubic_meter {volume_t, "cubic meter", "m3"};
        Uom number {number_t, "unit", "u"};

        EXPECT_EQ( liter.name() , "liter");
        EXPECT_EQ( liter.symbol() , "l");
        EXPECT_EQ( number.name() , "unit");
        EXPECT_EQ( number.symbol() , "u");
        EXPECT_EQ( liter.type() , cubic_meter.type());
        EXPECT_TRUE( liter.type() == cubic_meter.type());
        EXPECT_FALSE( liter.type() != cubic_meter.type());

        EXPECT_NE( liter.symbol() , cubic_meter.symbol());
        {
            EXPECT_NO_THROW({
                db.persist(liter);
                db.persist(cubic_meter);
                db.persist(number);
            });

            std::shared_ptr<UomType> power_t{ new UomType{"Power"}};
            Uom wat(power_t, "wat", "W");
            Uom kwat{power_t, "kilo wat", "kW"};

            EXPECT_EQ(wat.id, kwat.id);

            EXPECT_NO_THROW({
                db.persist(power_t);
                db.persist(wat);
                db.persist(kwat);
            });

            EXPECT_EQ( wat.type() , kwat.type());

            EXPECT_NE(wat.id, kwat.id);
            EXPECT_NE( wat.type() , liter.type());
        }
        trans.commit();
    }

}


}
