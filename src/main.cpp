#include <iostream>
#include <iomanip>
#include "../include/database.h"
#include "../include/date_validation.h"

void display_menu() {
    std::cout << std::setw(14) << std::setfill(' ') << "";
    std::cout << std::setw(26) << std::setfill('-') << "" << "Menu";
    std::cout << std::setw(26) << std::setfill('-') << "" << "\n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " D - Display the list \n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " A - Add a task \n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " E - Edit a task \n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " M - Mark a task completed\n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " Input your option: ";
}

void initialize_table(Database& db) {
    const char* create_table_query =
        "CREATE TABLE IF NOT EXISTS TASK ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "DESCRIPTION CHAR(50), "
        "DUE_DATE TEXT, "
        "COMPLETED INTEGER CHECK (COMPLETED IN (0,1))"
        ");";

    db.execute(create_table_query, "Table created successfully");
}

void insert_query(Database& db, const std::string& description, const std::string& due_date, int completed) {
  sqlite3_stmt* stmt;
  const char* sql = "INSERT INTO TASK (DESCRIPTION, DUE_DATE, COMPLETED) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, completed);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
          std::cout << "Data inserted successfully.\n";
        } else {
          std::cerr << "Error inserting data: " << sqlite3_errmsg(db.getDb()) << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.getDb()) << "\n";
    }
}

void handle_input(Database& db) {
    std::string description;
    std::string date;
    int completed = 0;

    std::cout << "Description: ";
    std::cin.ignore();
    getline(std::cin, description);

    std::cout << "\nDue Date (YYYY-MM-DD): ";
    std::cin >> date;
    while (!validateDate(date)) {
        std::cout << "\nDue Date (YYYY-MM-DD): ";
        std::cin >> date;
    }

    std::cout << "\nCompleted (0 for No, 1 for Yes): ";
    std::cin >> completed;
    while (completed != 1 && completed != 0) {
        std::cout << "\nCompleted (0 for No, 1 for Yes): ";
        std::cin >> completed;
    }

    insert_query(db, description, date, completed);
}

int main() {
    Database db("todolist.db");

    db.open();

    initialize_table(db);

    const char* select_query = "SELECT * FROM TASK;";
    db.query(select_query);

    char input = '0';
    display_menu();
    std::cin >> input;

    while (input != 'x' && input != 'X') {
        if (input == 'a' || input == 'A') {
            handle_input(db);
        }
        display_menu();
        std::cin >> input;
    }

    std::cout << std::endl;

    db.close();

    return 0;
}
