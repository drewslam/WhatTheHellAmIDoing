/* What the Hell Am I Doing?
 * Author: Andrew Souza
 *
 */
#include <iostream>
#include "../include/database.h"
#include "../include/date_validation.h"

void display_menu() {
    std::cout << std::string(14, ' ') << std::string(26, '-') << " Menu " << std::string(26, '-') << "\n";
    std::cout << std::string(28, ' ') << "*** D - Display the list\n";
    std::cout << std::string(28, ' ') << "*** A - Add a task\n";
    std::cout << std::string(28, ' ') << "*** E - Edit a task\n";
    std::cout << std::string(28, ' ') << "*** Z - Delete a task\n";
    std::cout << std::string(28, ' ') << "*** X - Exit the application\n";
    std::cout << std::string(21, ' ') << "------------------------------------------\n";
    std::cout << std::string(32, ' ') << "Please input your option: ";
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
        } else if (input == 'd' || input == 'D') {
            const char* select_query = "SELECT * FROM TASK;";
            db.query(select_query);
        } else if (input == 'z' || input == 'Z') {
            int taskID = -1;
            std::cout << "Input a task ID to delete: ";
            std::cin >> taskID;
            db.delete_task(taskID);
        } else if (input == 'e' || input == 'E') {
            char userOption;
            int taskID = -1;
            std::string newValue;
            std::cout << "Input the task ID: ";
            std::cin >> taskID;

            std::cout << "\nSelect from the following options:\n";
            std::cout << "'D' - Update Description\n";
            std::cout << "'T' - Update Due Date\n";
            std::cout << "'C' - Toggle Complete/Incomplete\n";
            std::cin >> userOption;

            if (userOption == 'd' || userOption == 'D') {
                std::cout << "Input your new description:\n";
                getline(std::cin, newValue);
                db.edit_task(taskID, 'd', newValue);
            } else if (userOption == 't' || userOption == 'T') {
                std::cout << "Input your new due date:\n";
                std::cin >> newValue;
                while (!validateDate(newValue)) {
                    std::cout << "\nDue Date (YYYY-MM-DD): ";
                    std::cin >> newValue;
                }
                db.edit_task(taskID, 't', newValue);
            } else if (userOption == 'c' || userOption == 'C') {
                db.edit_task(taskID, 'c', "");
            }
        } else {
            std::cerr << "Invalid option selected.\n";
        }
        display_menu();
        std::cin >> input;
    }

    std::cout << std::string(40, ' ') << "Goodbye!" << std::endl;

    db.close();

    return 0;
}
