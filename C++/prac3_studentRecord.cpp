// Write the Full C++ OOP program of the Student Record program using 
// Array of Structures with File Handling (CSV format).
// Submit the printed code to your class rep next week.

// -addRecord (Sorted in alphabetical)
// -deleteRecord
// -Display
// -Save
// -Retrieve


#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

#define MAX 100

struct Student {
    string name;
    float gpa;
};

class StudentRecord {
private:
    Student students[MAX];
    int count = 0;
    string filename;

    void sort_records() {
        for (int i = 1; i < count; i++) {
            Student key = students[i];
            int j = i - 1;
            while(j >= 0 && key.name < students[j].name){
                students[j + 1] = students[j];
                j--;
            }
            students[j + 1] = key;
        }
    }

    void print_footer() { //14
        cout << string(38, '=') << endl;
    }

public:
    StudentRecord() = default;
    StudentRecord(string file) : filename(file) {};

    void add_record(string name, float gpa) {
        if (count >= MAX) {
            cout << "The List is Full.\n";
            return;
        }
        students[count].name = name;
        students[count].gpa = gpa;
        count++;
    }

    void delete_record(string name) {
        if (count <= 0) {
            cout << "Student record are empty";
        }
        for (int i = 0; i < count; i++) {
            if(name == students[i].name) {
                for (int j = i; i < count - 1; j++) {
                    students[j] = students[j + 1];
                }
                count--;
            }
        }
    }

    void display() {
        cout << setw(5) << left << "No." 
        << setw(29)<< left << "Name" 
        << setw(4) << right << "GPA" << endl;

        for (int i = 0; i < count; i++) {
            cout << setw(5) << left << i + 1
            << setw(25) << left << students[i].name 
            << right << fixed << setprecision(2)
            << setw(8) << students[i].gpa << '\n';
        }
        print_footer();
    }

    void save() {
        fstream File;
        File.open(filename, ios::out);
        if (!File.is_open()) {
            cout << "Cannot open the File.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            File << students[i].name << ',' << students[i].gpa << endl;
        }
        File.close();
    }

    void retrieve() {
        fstream File;
        File.open(filename, ios::in);
        if (!File.is_open()) {
            cout << "File not found.\n";
            return;
        }

        string line;
        count = 0;
        while(getline(File,line) && count < MAX) {
            stringstream ss(line);
            string name;
            float gpa;

            getline(ss, name, ',');
            ss >> gpa;

            students[count].name = name;
            students[count].gpa = gpa;
            count++;
        }
        File.close();
    }
};

int main() {
    StudentRecord system("students.csv");
    system.retrieve();
    
    int choice;
    string name;
    float gpa;
    
    while (true) {
        cout << "\n======= Student Record System =======" << endl;
        cout << "1. Add Record" << endl;
        cout << "2. Delete Record" << endl;
        cout << "3. Display All Records" << endl;
        cout << "4. Save to File" << endl;
        cout << "5. Retrieve from File" << endl;
        cout << "6. Exit" << endl;
        cout << "=====================================" << endl;
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