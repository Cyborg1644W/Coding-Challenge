{#include <iostream>

using namespace std;

struct Record {
    string name;
    float grade;
    int studentId;
}

struct Node {
    Record data;
    Node *next;

    Node(Record x) {
        data = x;
        next = NULL;
    }
}

class Student {
private:
    Node *head;
public:
    Student() {
        head = NULL;
    } 

    void insertRecord(Record x){}
    void deleteRecord(Record x)
}

void Student::insertRecord(Record x) {
    Node *p, *q, *newNode;
    p = q = head;
    newNode = new Node();

    while( p != NULL && x.name > p->data.name) {
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

void Student::deleteRecord(Record x) {

} }