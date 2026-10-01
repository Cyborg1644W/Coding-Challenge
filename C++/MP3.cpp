//ANDRADA, Reindel T.
//BSCS - 2A
//MP3 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const string FILENAME = "students.csv";

struct Record {
    string name;
    int q1, q2, q3;
};

struct Node {
    Record data;
    Node *next;
    Node *prev;

    Node(Record x) {
        data = x;
        next = NULL;
        prev = NULL;
    }
};

class Student {
private:
    Node *head;
    Node *tail;
    float average(Record x);

    bool isEmpty() {return (head == NULL); }

public:
    Student() {
        head = NULL;
        tail = NULL;
    }
    
    ~Student() {
        Node *p;
        while(head != NULL) {
            p = head;
            head = head->next;
            delete(p);
        }
    }

    void addRec(Record x);
    void updateRec(string name);
    void delRec(string name);
    int locate(string name);
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
                
                if (list.locate(data.name) != -1) {
                    cout << "Record Already Exists!\n";
                    system("pause");
                    break;
                }
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

int Student::locate(string name) {
    Node *p = head;
    while(p != NULL && name != p->data.name) {
        p = p->next;
    }
    if(p == NULL) {
        return -1;
    } else {
        return 0;
    }
}

void Student::addRec(Record x) {
    Node *p, *q, *newNode;
    p = head;
    q = NULL;
    newNode = new Node(x);

    while(p != NULL && x.name > p->data.name) {
        q = p;
        p = p->next;
    }

    newNode->next = p;
    newNode->prev = q;

    if(q == NULL) {
        head = newNode;
    } else {
        q->next = newNode;
    }

    if(p == NULL) {
        tail = newNode;
    } else {
        p->prev = newNode;
    }
}

void Student::updateRec(string name) {
    Node* p = head;
    int newscr, choice;
    
    while(p != NULL && name != p->data.name) {
        p = p->next;
    }

    if(p == NULL) {
        cout << "The record of " << name << " was not found!\n";
        system("pause");
    } else {
        cout << "Record Found!\n";
        cout << left << setw(15) << "NAME" << setw(6) << "Q1" << setw(6) << "Q2" << setw(6) << "Q3" << endl;
        cout << setw(15) << p->data.name << setw(6) << p->data.q1 << setw(6) << p->data.q2 << setw(6) << p->data.q3 << endl << endl;
        
        cout << "Select score to update\n1.) Q1\n2.) Q2\n3.) Q3\n\nSelect (1-3): ";
        cin >> choice;

        switch(choice) {
            case 1:
                cout << "Input New Score: ";
                cin >> newscr;
                p->data.q1 = newscr;
                cout << "Record updated!\n";
                system("pause");
                break;
            case 2:
                cout << "Input New Score: ";
                cin >> newscr;
                p->data.q2 = newscr;
                cout << "Record updated!\n";
                system("pause");
                break;
            case 3:
                cout << "Input New Score: ";
                cin >> newscr;
                p->data.q3 = newscr;
                cout << "Record updated!\n";
                system("pause");
                break;
            default:
                cout << "Invalid choice\n";
                system("pause");
                break;
        }
    }
}

void Student::delRec(string name) {
    Node *p;
    p = head;
    while(p != NULL && name != p->data.name) {
        p = p->next;
    }

    if(p == NULL) {
        cout << name << " not Found!\n";
        system("pause");
    } else {
        if(p == tail && p == head) {
            head = NULL;
            tail = NULL;
        }
        else if(p == head) {
            head = head->next;
            head->prev = NULL;
        }
        else if(p == tail) {
            tail = tail->prev;
            tail->next = NULL;
        }
        else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
        }
        
        delete(p);
        cout << "Record of " << name << " successfully deleted!\n";
        system("pause");
    }
}

float Student::average(Record x) {
    float avg;
    avg = (x.q1 + x.q2 + x.q3) / 3.0;
    return avg;
}

void Student::display() {
    system("cls");
    float av;
    Node* p = head;
    
    cout << left << setw(15) << "NAME"
         << setw(6)  << "Q1"
         << setw(6)  << "Q2"
         << setw(6)  << "Q3"
         << setw(10) << "AVERAGE" << endl;

    while (p != NULL) {
        av = average(p->data);
        cout << left << setw(15) << p->data.name
             << setw(6) << p->data.q1
             << setw(6) << p->data.q2
             << setw(6) << p->data.q3
             << fixed << setprecision(2) << setw(10) << av << endl;
        p = p->next;
    }
    
    system("pause");
}

void Student::save() {
    ofstream file(FILENAME);
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
    ifstream file(FILENAME);
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
        
        Node *p, *q, *newNode;
        p = head; q = NULL; newNode = new Node(fd);
        while(p != NULL && fd.name > p->data.name) { q = p; p = p->next; }
        newNode->next = p; newNode->prev = q;
        if(q == NULL) head = newNode; else q->next = newNode;
        if(p == NULL) tail = newNode; else p->prev = newNode;
    }
    file.close();
}