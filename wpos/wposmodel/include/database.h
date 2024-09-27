// file      :	database.h
// birth     :  11/16/2023
// copiright :  Copyright (c) 2016-2023 WireSens Inc.
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>

namespace odb{
    class database;
    class transaction;
    using dbptr_t = std::unique_ptr<odb::database>;
}

using std::string;
namespace wpos{
class database{
public:
    static odb::database& instance();

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
    static odb::dbptr_t dbptr;
    static Connector cntr;

public:
    static const string SQLITE_FILE;
};
}
#endif // DATABASE_H
