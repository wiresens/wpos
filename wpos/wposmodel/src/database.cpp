// file      :	database.cpp
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include <database.h>
#include <exceptions.h>

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/mysql/database.hxx>

#include <odb/sqlite/database.hxx>
#include <odb/connection.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

std::unique_ptr<odb::database> database::db{nullptr};

database::Connector database::cntr{};
const string database::SqliteFile{"wpos.db"};

void database::loadConnector(Connector &cntr){
    Connector tmp_cntr{"SQLITE", "localhost","wpos","wpos","D0rc4566$", 5432};
    std::swap(cntr, tmp_cntr);
}

// Not thread safe
odb::database& database::instance(){
    if( !db) {
        loadConnector(cntr);

        if( cntr.driver == string("PGSQL") )
            db.reset(new odb::pgsql::database(cntr.user, cntr.pwd, cntr.db,cntr.host, cntr.port));

        else if( cntr.driver == string("MYSQL") )
            db.reset(new odb::mysql::database(cntr.user, cntr.pwd, cntr.db,cntr.host, cntr.port));

        else if( cntr.driver == string("SQLITE") )
        {
            auto mode = SQLITE_OPEN_READWRITE;

            FILE *device = fopen(SqliteFile.c_str(), "rw");

            if( !device) mode |= SQLITE_OPEN_CREATE;
            else fclose(device);

            db.reset( new odb::sqlite::database(SqliteFile, mode));
            if( !device){
                using namespace odb::core;
                connection_ptr connection(db->connection());
                connection->execute("PRAGMA foreign_keys=OFF");
                    transaction trans(connection->begin());
                        schema_catalog::create_schema(*db);
                    trans.commit();
               connection->execute("PRAGMA foreign_keys=ON");
            }
        }
        else throw wpos::unknown_driver_exception{};
    }
    return *db;
}
