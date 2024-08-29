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
    bool edit_task(int id, char option, const std::string& new_value);
    bool query(const char* sql);


private:
    std::string db_filename;
    sqlite3* db = nullptr;

    static int callback(void* data, int argc, char** argv, char** azColName);
    bool update_description(int id, const std::string& new_description);
    bool update_due_date(int id, const std::string& new_due_date);
    bool toggle_completed(int id);
};

#endif
