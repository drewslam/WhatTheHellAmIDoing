#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

class Database {
public:
    Database(const char* filename);
    bool open();
    void close();
    bool execute(const char* sql, const char* success_message);
    bool query(const char* sql);

private:
    const char* db_filename;
    sqlite3* db;

    static int callback(void* data, int argc, char** argv, char** azColName);
};

#endif
