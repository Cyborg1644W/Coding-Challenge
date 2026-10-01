//ANDRADA, Reindel T.
//BSCS - 2A

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Record {
    string name;
    int q1, q2, q3;
};

struct Node {
    Record data;
    Node* next;
    
    Node(Record x) : data(x), next(NULL) {}
};

class Student {
private:
    Node* head;

    bool isEmpty() { return (head == NULL); }

    Node* find(string name) { 
        Node* p = head;
        while (p != NULL && p->data.name != name) {
            p = p->next;
        }
        return p;
    }

public:
    Student() {
        head = NULL;
    }

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
    string name;
    Student list;

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
                cin >> name;
                list.updateRec(name);
                list.save();
                break;
            case 3:
                cout << "Input name to delete: ";
                cin >> name;
                list.delRec(name);
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
    int num;
    system("cls");
    cout << "STUDENT RECORD SYSTEM\n";
    cout << "1. Add Record (Sorted by name)\n";
    cout << "2. Update Record\n";
    cout << "3. Delete Record\n";
    cout << "4. Display\n";
    cout << "5. Exit\n";
    cout << "Select[1-5]: ";
    cin >>  num;
    return num;
}

void Student::addRec(Record x) {
    if (find(x.name) != NULL) {
        cout << x.name << " already exist\n";
        return;
    }
    Node *newNode = new Node(x);
    
    if (head == NULL || head->data.name > x.name) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* p = head;
        Node* q = NULL;
        
        while (p != NULL && p->data.name < x.name) {
            q = p;
            p = p->next;
        }
        
        newNode->next = p;
        q->next = newNode;
    }
}

void Student::updateRec(string name) {
    if (isEmpty()) {
        cout << "Nothing to update.\n";
        system("pause");
        return;
    }

    Node* p = find(name);
    if (p == NULL) {
        cout << "Not Found.\n";
        system("pause");
        return;
    }

    cout << "Current Q1: " << p->data.q1 << " | New Q1: ";
    cin >> p->data.q1;
    cout << "Current Q2: " << p->data.q2 << " | New Q2: ";
    cin >> p->data.q2;
    cout << "Current Q3: " << p->data.q3 <<  " | New Q3: ";
    cin >> p->data.q3;

    cout << "Record updated.\n";
    system("pause");
}

void Student::delRec(string name) {
    if (isEmpty()) {
        cout << "Nothing to delete.\n";
        system("pause");
        return;
    }

    Node* p = head;
    Node* q = NULL;
    
    while (p != NULL && p->data.name != name) {
        q = p;
        p = p->next;
    }
    
    if (p == NULL) {
        cout << "Not Found.\n";
        system("pause");
        return;
    }
    
    if (p == head)
        head = head->next;
    else
        q->next = p->next;
    
    delete p;
}

void Student::display() {
    system("cls");
    cout << left << setw(15) << "NAME"
         << setw(6)  << "Q1"
         << setw(6)  << "Q2"
         << setw(6)  << "Q3"
         << setw(10) << "AVERAGE" << endl;

    Node* p = head;
    while (p != NULL) {
        double avg = (p->data.q1 + p->data.q2 + p->data.q3) / 3.0;
        cout << left << setw(15) << p->data.name
             << setw(6) << p->data.q1
             << setw(6) << p->data.q2
             << setw(6) << p->data.q3
             << fixed << setprecision(2) << setw(10) << avg << endl;
        p = p->next;
    }
    
    system("pause");
}

void Student::save() {
    ofstream file("students.csv");
    if (!file) {
        cout << "File Error.\n";
        return;
    }
    
    Node* p = head;
    while (p != NULL) {
        file << p->data.name << "," << p->data.q1 << "," << p->data.q2 << "," << p->data.q3 << endl;
        p = p->next;
    }
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