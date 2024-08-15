#include <iostream>
#include <iomanip>
#include "../include/database.h"

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
        "ID INT PRIMARY KEY NOT NULL, "
        "DESCRIPTION CHAR(50), "
        "DUE_DATE TEXT, "
        "COMPLETED INTEGER CHECK (COMPLETED IN (0,1))"
        ");";

    db.execute(create_table_query, "Table created successfully");
}

int main() {
    Database db("todolist.db");

    db.open();

    initialize_table(db);

    const char* insert_data_query =
        "INSERT INTO TASK (DESCRIPTION, DUE_DATE, COMPLETED) "
        "VALUES ('Finish this application', '2024-12-31', 0);";

    db.execute(insert_data_query, "Data inserted successfully.");

    const char* select_query = "SELECT * FROM TASK;";
    db.query(select_query);

    display_menu();
    char input;
    std::cin >> input;

    std::cout << std::endl;

    db.close();

    return 0;
}
