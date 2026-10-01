//ANDRADA, Reindel T.
//BSCS -2A

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define MAX 50

struct Record {
    string name;
    int q1, q2, q3;
};

class Student {
private:
    Record pd[MAX];
    int last;

    bool isFull()  { return (last == MAX - 1); }
    bool isEmpty() { return (last == -1); }
    int  locate(string name) {
        for (int i = 0; i <= last; i++)
            if (pd[i].name == name)
                return i;
        return -1;
    }
    void insertionSort();

public:
    void makenull();
    void addRec(Record x);
    void updateRec(string name);
    void delRec(string name);
    void display();
    void save();
    void retrieve();
};

int menu();

int main() {
    Record data;
    string target;
    Student list;

    list.makenull();
    list.retrieve();

    while (true) {
        switch (menu()) {
            case 1:
                cout << "Input name: ";
                cin >> data.name;
                cout << "Input Q1: "; cin >> data.q1;
                cout << "Input Q2: "; cin >> data.q2;
                cout << "Input Q3: "; cin >> data.q3;
                list.addRec(data);
                list.save();
                break;
            case 2:
                cout << "Input name to update: ";
                cin >> target;
                list.updateRec(target);
                list.save();
                break;
            case 3:
                cout << "Input name to delete: ";
                cin >> target;
                list.delRec(target);
                list.save();
                break;
            case 4:
                list.display();
                break;
            case 5:
                list.save();
                cout << "Records saved. Exiting.\n";
                exit(0);
            default:
                cout << "Invalid input.\n";
                system("pause");
        }
    }
    return 0;
}

int menu() {
    int op;
    system("cls");
    cout << "STUDENT RECORD SYSTEM\n";
    cout << "1. Add Record (Sorted by name)\n";
    cout << "2. Update Record\n";
    cout << "3. Delete Record\n";
    cout << "4. Display\n";
    cout << "5. Exit\n";
    cout << "Select[1-5]: ";
    cin >> op;
    return op;
}

void Student::makenull() {
    last = -1;
}

void Student::insertionSort() {
    for (int i = 1; i <= last; i++) {
        Record key = pd[i];
        int j = i - 1;
        while (j >= 0 && pd[j].name > key.name) {
            pd[j + 1] = pd[j];
            j--;
        }
        pd[j + 1] = key;
    }
}

void Student::addRec(Record x) {
    if (isFull()) {
        cout << "List is full.\n";
        system("pause");
        return;
    }

    if (locate(x.name) != -1) {
        cout << "A record with that name already exists.\n";
        system("pause");
        return;
    }
    
    last++;
    pd[last] = x;

    insertionSort();
}

void Student::updateRec(string name) {
    if (isEmpty()) {
        cout << "Nothing to update.\n";
        system("pause");
        return;
    }

    int p = locate(name);
    if (p < 0) {
        cout << "Not Found.\n";
        system("pause");
        return;
    }

    cout << "Current Q1: " << pd[p].q1 << " | New Q1: ";
    cin >> pd[p].q1;
    cout << "Current Q2: " << pd[p].q2 << " | New Q2: ";
    cin >> pd[p].q2;
    cout << "Current Q3: " << pd[p].q3 << " | New Q3: ";
    cin >> pd[p].q3;

    cout << "Record updated.\n";
    system("pause");
}

void Student::delRec(string name) {
    int i, p;
    if (isEmpty()) {
        cout << "Nothing to delete.\n";
        system("pause");
        return;
    }

    p = locate(name);
    if (p < 0) {
        cout << "Not Found.\n";
        system("pause");
        return;
    }

    for (i = p; i < last; i++)
        pd[i] = pd[i + 1];
    last--;
}

void Student::display() {
    system("cls");
    cout << left << setw(15) << "NAME"
         << setw(6)  << "Q1"
         << setw(6)  << "Q2"
         << setw(6)  << "Q3"
         << setw(10) << "AVERAGE" << endl;

    for (int i = 0; i <= last; i++) {
        double avg = (pd[i].q1 + pd[i].q2 + pd[i].q3) / 3.0;
        cout << left << setw(15) << pd[i].name
             << setw(6) << pd[i].q1
             << setw(6) << pd[i].q2
             << setw(6) << pd[i].q3
             << fixed << setprecision(2) << setw(10) << avg << endl;
    }
    system("pause");
}

void Student::save() {
    ofstream file("students.csv");
    if (!file) {
        cout << "File Error.\n";
        return;
    }
    for (int i = 0; i <= last; i++)
        file << pd[i].name << "," << pd[i].q1 << "," << pd[i].q2 << "," << pd[i].q3 << endl;
    file.close();
}

void Student::retrieve() {
    ifstream file("students.csv");
    if (!file)
        return; 

    string line, field;
    Record fd;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        getline(ss, fd.name, ',');
        getline(ss, field, ','); fd.q1 = stoi(field);
        getline(ss, field, ','); fd.q2 = stoi(field);
        getline(ss, field, ','); fd.q3 = stoi(field);
        addRec(fd);
    }
    file.close();
}