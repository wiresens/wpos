// file      :	party_test.cpp
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#include <core/party.h>
#include <core/party-odb.hxx>
#include <database.h>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>

namespace {
// The fixture for testing Address class.
class AddressTest : public ::testing::Test {
protected:
    odb::database& db{wpos::database::instance()};
};

class PersonTest : public ::testing::Test {
protected:
    odb::database& db{wpos::database::instance()};
};

using namespace wpos::model;

TEST_F(AddressTest, ConstructorPersist)
{

    Address gillesAddr{"670275339", "Cameroon","Yaounde"};
    Address alidaAddr{"694607620", "Cameroon","Yaounde"};
    Address raoulAddr{"698877455", "Cameroon","Douala"};

    odb::transaction trans(db.begin());
    EXPECT_NO_THROW({
        db.persist(gillesAddr);
        db.persist(alidaAddr);
        db.persist(raoulAddr);
    });
    trans.commit();
}

TEST_F(PersonTest, ConstructorPersist)
{
    {
        using query = odb::query<Address>;

        odb::transaction trans(db.begin());
        auto gillesAddr = db.query_one<Address>(query::id == 1);
        auto alidaAddr  = db.query_one<Address>(query::id == 2);
        auto raoulAddr  = db.query_one<Address>(query::id == 3);

        Person gilles{gillesAddr, Person::Male,"BPG","BENE POUGOUE"};
        Person alida{alidaAddr, Person::Female,"CTA","CHOUMI TCHAMBA"};
        Person raoul{raoulAddr,Person::Male,"KRC","KWETCHEU"};

        EXPECT_EQ( raoul.gender() , gilles.gender());
        EXPECT_EQ( alida.gender() , Person::Female);

        EXPECT_EQ( alida.address().city() , gilles.address().city());
        EXPECT_EQ( alida.address().country() , gilles.address().country());
        EXPECT_EQ( alida.address().country() , raoul.address().country());
        EXPECT_EQ( raoul.address().country() , gilles.address().country());

        EXPECT_NE( raoul.address().city() , gilles.address().city());
        EXPECT_NE( alida.address().principalPhone() , gilles.address().principalPhone());

        EXPECT_NO_THROW({
            db.persist(gilles);
            db.persist(alida);
            db.persist(raoul);
        });
        trans.commit();
    }
}

}
