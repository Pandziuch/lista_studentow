#include <iostream>
#include <string>
#include "sqlite3.h"

class DatabaseManager {
public:
    DatabaseManager() {
        sqlite3_open("school.db", &db);
        std::cout << "Opened database successfully\n";
        createTables();
    }

    ~DatabaseManager() {
        sqlite3_close(db);
    }

    void createTables() {
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS students ("
                         "id INTEGER PRIMARY KEY,"
                         "first_name TEXT NOT NULL,"
                         "last_name TEXT NOT NULL,"
                         "average REAL,"
                         "grade INTEGER"
                         ");", NULL, 0, NULL);
    }

    void addStudent(int id, const std::string& firstName, const std::string& lastName, float average, int grade) {
        std::string sql = "INSERT INTO students (id, first_name, last_name, average, grade) VALUES (" + std::to_string(id) + ", '" + firstName + "', '" + lastName + "', " + std::to_string(average) + ", " + std::to_string(grade) + ");";
        sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
        std::cout << "Added student successfully\n";
    }

    void removeStudent(int id) {
        std::string sql = "DELETE FROM students WHERE id = " + std::to_string(id) + ";";
        sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
        std::cout << "Removed student successfully\n";
    }

    void displayStudents() {
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "SELECT id, first_name, last_name, average, grade FROM students;", -1, &stmt, NULL);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* firstName = sqlite3_column_text(stmt, 1);
            const unsigned char* lastName = sqlite3_column_text(stmt, 2);
            float average = sqlite3_column_double(stmt, 3);
            int grade = sqlite3_column_int(stmt, 4);
            std::cout << "ID: " << id << ", Name: " << firstName << " " << lastName
                      << ", Average: " << average << ", Grade: " << grade << std::endl;
        }
        sqlite3_finalize(stmt);
    }

private:
    sqlite3* db;
};

int main() {
    DatabaseManager dbManager;

    int choice;
    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Add student\n";
        std::cout << "2. Display students\n";
        std::cout << "3. Remove student\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                std::string firstName, lastName;
                float average;
                int grade;
                std::cout << "Enter ID: ";
                std::cin >> id;
                std::cout << "Enter first name: ";
                std::cin >> firstName;
                std::cout << "Enter last name: ";
                std::cin >> lastName;
                std::cout << "Enter average: ";
                std::cin >> average;
                std::cout << "Enter grade: ";
                std::cin >> grade;
                dbManager.addStudent(id, firstName, lastName, average, grade);
                break;
            }
            case 2:
                std::cout << "Students:\n";
                dbManager.displayStudents();
                break;
            case 3: {
                int id;
                std::cout << "Enter ID of student to remove: ";
                std::cin >> id;
                dbManager.removeStudent(id);
                break;
            }
            case 4:
                std::cout << "Exiting program\n";
                return 0;
            default:
                std::cout << "Invalid choice\n";
        }
    }

    return 0;
}
