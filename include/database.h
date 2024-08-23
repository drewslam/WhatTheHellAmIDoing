#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>

class Database {
public:
    Database(const char* filename);
    ~Database();
    sqlite3* getDb() const;
    bool open();
    void close();
    bool execute(const char* sql, const char* success_message);
    bool insert_task(const std::string& description, const std::string& due_date, int completed);
    bool delete_task(int id);
    bool query(const char* sql);


private:
    const char* db_filename;
    sqlite3* db = nullptr;

    static int callback(void* data, int argc, char** argv, char** azColName);
};

#endif
