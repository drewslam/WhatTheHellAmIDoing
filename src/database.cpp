#include "../include/database.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>

Database::Database(const char* filename) { 
    const char* home_dir = std::getenv("HOME");
    if (!home_dir) {
        throw std::runtime_error("Failed to get the HOME environment variable.");
    }

    // Construct the path to the database file
    std::string db_path = std::string(home_dir) + "/.wthaid/";

    // Ensure the directory exists
    std::filesystem::create_directory(db_path);

    db_path += filename;

    std::cout << "Database path: " << db_path << std::endl;

    db_filename = db_path;

    if (!open()) {
        throw std::runtime_error("Failed to open database.");
    }
}

Database::~Database() {
    if (db) { sqlite3_close(db); }
}

sqlite3* Database::getDb() const {
    return db;
}

bool Database::open() {
    int exit = sqlite3_open(db_filename.c_str(), &db);
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
    char* messageError = nullptr;
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

bool Database::insert_task(const std::string& description, const std::string& due_date, int completed) {
    const char* sql = "INSERT INTO TASK (DESCRIPTION, DUE_DATE, COMPLETED) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, completed);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Data inserted successfully.\n";
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error inserting data: " << sqlite3_errmsg(getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(getDb()) << "\n";
    }
    return false;
}

bool Database::delete_task(int id) {
    const char* sql = "DELETE FROM TASK WHERE ID = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Task deleted successfully.\n";
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error deleting task: " << sqlite3_errmsg(getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(getDb()) << "\n";
    }

    return false;
}

bool Database::edit_task(int id, char option, const std::string& new_value) {
    switch (option) {
        case 'd': return update_description(id, new_value);
        case 't': return update_due_date(id, new_value);
        case 'c': return toggle_completed(id);
        default:
            std::cerr << "Invalid option.\n";
            return false;
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
    (void)data;
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << std::endl;
    return 0;
}

bool Database::update_description(int id, const std::string& new_description) {
    const char* sql = "UPDATE TASK SET DESCRIPTION = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, new_description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Task updated successfully.\n";
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error updating task: " <<sqlite3_errmsg(getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(getDb()) << "\n";
    }

    return false;
}

bool Database::update_due_date(int id, const std::string& new_due_date) {
    const char* sql = "UPDATE TASK SET DUE_DATE = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, new_due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Task updated successfully.\n";
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error updating task: " <<sqlite3_errmsg(getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(getDb()) << "\n";
    }

    return false;
}

bool Database::toggle_completed(int id) {
    const char* sql = "UPDATE TASK SET COMPLETED = 1 - COMPLETED WHERE ID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Task updated successfully.\n";
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error updating task: " <<sqlite3_errmsg(getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(getDb()) << "\n";
    }

    return false;
}
