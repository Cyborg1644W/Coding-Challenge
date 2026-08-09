// Write the Full C++ OOP program of the Student Record program using 
// Array of Structures with File Handling (CSV format).
// Submit the printed code to your class rep next week.

// -addRecord (Sorted in alphabetical)
// -deleteRecord
// -Display
// -Save
// -Retrieve


#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#define MAX 100

struct Student {
    string name;
    float gpa;
};

class StudentRecord {
private:
    Student students[MAX];
    int count;
    string filename;

    void sort_records() {
        for (int i = 1; i < count; i++) {
            Student key = students[i];
            int j = i - 1;

            while (j >= 0 && students[j].name > key.name) {
                students[j + 1] = students[j];
                j--; 
            }
            students[j + 1] = key;
        }
    }

    void print_footer() { //14
        cout << string(30, '=') << endl;
    }

public:
    StudentRecord() = default;
    StudentRecord (string file) : filename(file) {} 

    void add_record(string name, float gpa) {
        if (count >= MAX) {
            cout << "Record list is full.\n";
            return;
        }
        students[count].name = name;
        students[count].gpa = gpa;
        count++;
        sort_records();
    }

    void delete_record(string name) {
        if (count <= 0) {
            cout << "Record list is empty.\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            if (students[i].name == name) {
                for (int j = i; i < count - 1; j++) {
                    students[i].name = students[i + 1].name;
                }
                count--;
                cout << "Record deleted.\n";
            }
        }
        cout << "Student not found.\n";
    }

    void display() {
        cout << string(8, '=') << "STUDENT RECORD" << string(8, '=') << endl;
        if (count <= 0) {
            cout << "No student records found.\n";
        } else {
            cout << setw(5) << "No."
            << setw(25) << left << "Name"
            << setw(8) << right << "GPA" << '\n';

            for (int i = 0; i < count - 1; i++) {
                cout << left
                << setw(5) << i + 1
                << setw(25) << students[i].name
                << right
                << fixed << setprecision(2)
                << setw(8) << students[i].gpa
                << '\n';
            }
        }
        print_footer();
    }
    void save() {

    }
    void retrieve() {

        }
};

int main() {
    StudentRecord system("students.csv");

    // Retrieve existing records
    system.retrieve();
    
    int choice;
    string name;
    float gpa;
    
    while (true) {
        cout << "\n===== Student Record System =====" << endl;
        cout << "1. Add Record" << endl;
        cout << "2. Delete Record" << endl;
        cout << "3. Display All Records" << endl;
        cout << "4. Save to File" << endl;
        cout << "5. Retrieve from File" << endl;
        cout << "6. Exit" << endl;
        cout << "=================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();  // ignore newline
        
        switch (choice) {
            case 1:
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter GPA: ";
                cin >> gpa;
                system.add_record(name, gpa);
                break;
                
            case 2:
                cout << "Enter name to delete: ";
                getline(cin, name);
                system.delete_record(name);
                break;
                
            case 3:
                system.display();
                break;
                
            case 4:
                system.save();
                break;
                
            case 5:
                system.retrieve();
                break;
                
            case 6:
                cout << "Exiting... Goodbye!" << endl;
                return 0;
                
            default:
                cout << "Invalid choice!" << endl;
        }
    }
    
    return 0;
}
