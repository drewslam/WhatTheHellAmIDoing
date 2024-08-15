#include "../include/database.h"

int main() {
    Database db("todolist.db");

    db.open();

    const char* create_table_query =
        "CREATE TABLE IF NOT EXISTS TASK ("
        "ID INT PRIMARY KEY NOT NULL, "
        "DESCRIPTION CHAR(50), "
        "DUE_DATE TEXT, "
        "COMPLETED INTEGER CHECK (COMPLETED IN (0,1))"
        ");";

    db.execute(create_table_query, "Table created successfully");

    const char* insert_data_query =
        "INSERT INTO TASK (ID, DESCRIPTION, DUE_DATE, COMPLETED) "
        "VALUES (1, 'Finish this application', '2024-12-31', 0);";

    db.execute(insert_data_query, "Data inserted successfully.");

    const char* select_query = "SELECT * FROM TASK;";
    db.query(select_query);

    db.close();

    return 0;
}
