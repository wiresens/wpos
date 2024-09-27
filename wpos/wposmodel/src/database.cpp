// file      :	database.cpp
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "database.h"
#include "exceptions.h"

#include <odb/core.hxx>
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/connection.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

#include <iostream>

odb::dbptr_t wpos::database::dbptr{nullptr};

wpos::database::Connector wpos::database::cntr{};
const string wpos::database::SQLITE_FILE{"wpos.db"};

void wpos::database::loadConnector(Connector &cntr){
    Connector tmp_cntr{"SQLITE", "localhost","wpos","wpos","D0rc4566$", 5432};
    std::swap(cntr, tmp_cntr);
}

// Not thread safe
odb::database& wpos::database::instance(){
    std::cout << "In instance \n";
    if( !dbptr){
        loadConnector(cntr);

        if( cntr.driver == string("PGSQL") )
            dbptr.reset(new odb::pgsql::database(cntr.user, cntr.pwd, cntr.db,cntr.host, cntr.port));

        else if( cntr.driver == string("MYSQL") )
            dbptr.reset(new odb::mysql::database(cntr.user, cntr.pwd, cntr.db,cntr.host, cntr.port));

        else if( cntr.driver == string("SQLITE") )
        {
            auto mode = SQLITE_OPEN_READWRITE;

            FILE *device = fopen(SQLITE_FILE.c_str(), "rw");

            if( !device) mode |= SQLITE_OPEN_CREATE;
            else {
                fclose(device);
                std::cout << "Using found \"" << SQLITE_FILE << "\" sqlite database file\n";
            }

            dbptr.reset( new odb::sqlite::database(SQLITE_FILE, mode));
            if( !device){
                std::cout << "sqlite database file \"" << SQLITE_FILE << "\" not present, creating new schema\n";
                using namespace odb::core;
                connection_ptr connection(dbptr->connection());
                connection->execute("PRAGMA foreign_keys=OFF");
                    transaction trans(connection->begin());
                        schema_catalog::create_schema(*dbptr);
                    trans.commit();
               connection->execute("PRAGMA foreign_keys=ON");
            }
        }
        else throw wpos::unknown_driver_exception{};
    }
    return *dbptr;
}
