// file      :	user_test.cpp
// birth     :  11/23/2023
// copyright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include <core/user.h>
#include <core/user-odb.hxx>
#include <core/session_manager.h>
#include <database.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <gtest/gtest.h>

namespace {
// The fixture for testing User class.
class UserTest : public ::testing::Test {
protected:
    odb::database& db{ wpos::database::instance()};
};

using namespace wpos::model;

TEST_F(UserTest, ConstructorPersist)
{
    using query = odb::query<Person>;
    odb::transaction trans(db.begin());
        auto gilles = db.query_one<Person>(
            query::code == "BPG" &&
            query::name == "BENE POUGOUE"
        );

        auto alida = db.query_one<Person>(
            query::code == "CTA" &&
            query::name == "CHOUMI TCHAMBA"
        );

        auto raoul = db.query_one<Person>(
            query::code == "KRC" &&
            query::name == "KWETCHEU"
        );

        gilles->setFirstName("Gilles");
        alida->setFirstName("Alida");
        raoul->setFirstName("Raoul Cyprien");

        User gilles_user{gilles, "gilles", "D0rc4566$"};
        User alida_user{alida,"alida", "D0alida$"};
        User raoul_user{raoul,"raoul", "D0raoul$"};

        EXPECT_NO_THROW({
            db.update(gilles);
            db.update(alida);
            db.update(raoul);

            db.persist(gilles_user);
            db.persist(alida_user);
            db.persist(raoul_user);
        });
    trans.commit();
}

TEST_F(UserTest, Authenticate)
{
    {
        wpos::SessionManager sm;
        odb::transaction trans(db.begin());
            UserPtr gilles = sm.authenticate("gilles","D0rc4566$");
            UserPtr alida = sm.authenticate("alida","D0alida$");
            UserPtr raoul = sm.authenticate("raoul","D0raoul$");

            EXPECT_EQ( raoul->employee().gender() , gilles->employee().gender());
            EXPECT_EQ( alida->employee().gender() , Person::Female);

            EXPECT_EQ( alida->employee().address().city() , gilles->employee().address().city());
            EXPECT_EQ( alida->employee().address().country() , gilles->employee().address().country());
            EXPECT_EQ( alida->employee().address().country() , raoul->employee().address().country());
            EXPECT_EQ( raoul->employee().address().country() , gilles->employee().address().country());

            EXPECT_NE( raoul->employee().address().city() , gilles->employee().address().city());
            EXPECT_NE( alida->employee().address().principalPhone() , gilles->employee().address().principalPhone());

            sm.logOut(*gilles);
            sm.logOut(*alida);
            sm.logOut(*raoul);

            EXPECT_NO_THROW({
                db.update(gilles);
                db.update(alida);
                db.update(raoul);
            });
        trans.commit();
    }
}

}


