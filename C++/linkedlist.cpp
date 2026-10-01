#include <iostream>
using namespace std;

struct Record {
    string name;
    int age;
};

struct Node {
    Record data;
    Node *next;

    Node(Record x) { // Node(Record x) : data(x), next(NULL) {}
        data = x;
        next = NULL;
    }
};  

class Links { 
private:
    Node *head;
public:
    Links() { // Constructor for makenull
        head = NULL;
    }

    ~Links() {
        Node *p;
        while (head != NULL) {
            p = head;
            head = head->next;
            delete (p);
        }
    }
    void add(Record x);
    void del(string name);
    void display();
};  

void Links::add(Record x) {
    Node *p, *q, *newNode;
    p = q = head;
    newNode = new Node(x);

    // locate the right position of x
    while (p != NULL && x.name > p->data.name) {
        q = p;
        p = p->next;
    }
    if (p == head) // First element
        head = newNode;
    else
        q->next = newNode;
    newNode->next = p;
}

void Links::del(string name) {
    Node *p, *q;
    p = q = head;

    while (p != NULL && name != p->data.name) {
        q = p;
        p = p->next;
    }

    if (p == NULL) {
        cout << "Record of " << name << " Not found.\n";
        system("pause");
    } else {
        if (p == head)
            head = head->next;
        else
            q->next = p->next;
        delete p;
    }
}

void Links::display() {
    int i = 1;
    Node *p = head;
    system("cls");

    cout << "  Name    Age\n";
    while (p != NULL) {
        cout << i++ << ".) " << p->data.name << "   " << p->data.age << endl;
        p = p->next; // move to next node
    }
    system("pause");
}

int main() {
    Links lst;
    Record d;

    d.name = "Zorro";
    d.age = 12;
    lst.add(d);

    d.name = "Melchor";
    d.age = 33;
    lst.add(d);

    d.name = "Gaspar";
    d.age = 22;
    lst.add(d);

    lst.display();
    lst.del("hestas");

    return 0;
}