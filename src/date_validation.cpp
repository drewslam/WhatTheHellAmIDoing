#include <iostream>
#include <regex>
#include <string>

#include "../include/date_validation.h"

// Function to check if a year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to validate the date format and value
bool validateDate(const std::string& date) {
    // Regular expression for YYYY-MM-DD
    const std::regex date_pattern("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");

    // Check if the date matches the pattern
    if (!std::regex_match(date, date_pattern)) {
        std::cerr << "Error: Date must be in YYYY-MM-DD format." << std::endl;
        return false;
    }

    // Extract year, month, and day from the date string
    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    // Check if the day is valid for the given month
    if (month == 2) {
        if (isLeapYear(year)) {
            if (day > 29) {
                std::cerr << "Error: Invalid date. February " << year << " has only 29 days." << std::endl;
                return false;
            }
        } else if (day > 28) {
            std::cerr << "Error: Invalid date. February " << year << " has only 28 days." << std::endl;
            return false;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            std::cerr << "Error: Invalid date. " << month << " has only 30 days." << std::endl;
            return false;
        }
    }

    // If all checks pass, the date is valid
    return true;
}

