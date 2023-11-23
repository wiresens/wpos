// file      :	database.h
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef DATABASE_H
#define DATABASE_H

#include <odb/core.hxx>
#include <string>
#include <memory>

using std::string;

class database{
public:
    static odb::database &instance();
    virtual ~database();

//    struct Connector{
//        const string driver;
//        const string host;
//        const string db;
//        const string user;
//        const string pwd;
//        const uint16_t port;
//    };

    struct Connector{
        string driver;
        string host;
        string db;
        string user;
        string pwd;
        uint16_t port;
    };

private:
    database() = delete;
    static void loadConnector(Connector &cntr);

private:
    static std::unique_ptr<odb::database> db;
    static Connector cntr;

public:
    static const string SqliteFile;
};

#endif // DATABASE_H
