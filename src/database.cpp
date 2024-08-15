#include "../include/database.h"
#include <iostream>

Database::Database(const char* filename) : db_filename(filename) {}

bool Database::open() {
    int exit = sqlite3_open(db_filename, &db);
    if (exit) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << "\n";
        return false;
    } else {
        std::cout << "Opened Database Successfully!\n";
        return true;
    }
}

void Database::close() {
    sqlite3_close(db);
}

bool Database::execute(const char* sql, const char* success_message) {
    char* messageError;
    int exit = sqlite3_exec(db, sql, nullptr, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Execute: " << messageError << "\n";
        sqlite3_free(messageError);
        return false;
    } else {
        std::cout << success_message << "\n";
        return true;
    }
}

bool Database::query(const char* sql) {
    char* messageError;
    int exit = sqlite3_exec(db, sql, callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Query: " << messageError << "\n";
        sqlite3_free(messageError);
        return false;
    }
    return true;
}

int Database::callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << std::endl;
    return 0;
}
