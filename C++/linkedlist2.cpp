#include <iostream>

using namespace std;

struct Record {
    string name;
    int q1;
    int q2;
    int q3;
}

struct Node {
    Record data;
    Node *next;
    Node (Record x) : data(x){};
}

class Student {
    private:
    Node *head;
    public:
    Student() {
        head = NULL;
    }
    ~Student() {
        Node *p;
        while(p != NULL) {
            p = head;
            head = p->next;
            delete(p);
        }
    }

    void add(Record x){}   
    void delete(string name){}   
    void retrieve(){}   
    void save(){}
}

void Student::add(Record x) {
    Node *p, *q, *newnode;
    p = q = head;
    newNode = new Node(x);

    while(p != NULL && x.name > p->data.name) {
        q = p;
        p = p->next;
    }

    if (p == head) {
        head = newNode; 
    } else {
        q->next = newNode;
    }
    newNode->next = p;
}

void Student::delete(string name) {
    Node *p, *q;
    p = q = head;

    while(p != NULL && name > p->data.name) {
        q =  p;
        ap = p->next;
    }

    if (p = head) {
        head = head->next;
    } else {
        q->next = p->next;
    }
    delete(p);
}

void Student::save() {}