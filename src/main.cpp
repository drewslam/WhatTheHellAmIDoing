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
    std::cout << " Z - Delete a task\n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " M - Mark a task completed\n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " X - Exit the application\n";
    std::cout << std::setw(24) << std::setfill(' ') << "" << "***";
    std::cout << " Input your option: ";
}

void initialize_table(Database& db) {
    const char* create_table_query =
        "CREATE TABLE IF NOT EXISTS TASK ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "DESCRIPTION TEXT, "
        "DUE_DATE TEXT, "
        "COMPLETED INTEGER CHECK (COMPLETED IN (0,1))"
        ");";

    db.execute(create_table_query, "Table created successfully");
}

bool handle_input(Database& db) {
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

    return db.insert_task(description, date, completed);
}

int main() {
    Database db("todolist.db");

    db.open();

    initialize_table(db);

    char input = '0';
    display_menu();
    std::cin >> input;

    while (input != 'x' && input != 'X') {
        if (input == 'a' || input == 'A') {
            handle_input(db);
        }
        if (input == 'd' || input == 'D') {
            const char* select_query = "SELECT * FROM TASK;";
            db.query(select_query);
        }
        if (input == 'z' || input == 'Z') {
            int userInput = -1;
            std::cout << "Input a task ID to delete: ";
            std::cin >> userInput;
            db.delete_task(userInput);
        }
        
        display_menu();
        std::cin >> input;
        if (input == 'x' || input == 'X') {
            std::cout << "Goodbye!" << std::endl;
        }
    }

    db.close();

    return 0;
}
