#include <iostream>

using namespace std;

struct Record {
    String name;
    int q1;
    int q2;
    int q3;
}

struct Node {
    Record data; 
    node *next;

    Node (Record x) : data(x){};
}

class Student {
private:
    Node *head;

    Student () {
        head = NULL;
    }

    ~Student {
        Node *p;
        while(p != NULL) {
            p = head;
            head = p->next;
            delete(p);

        }
    }
}