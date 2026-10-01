#include <iostream>
using namespace std;

#define MAX 5

class CircularQueue {
private:
    int myArr[MAX]; 
    int f; 
    int r; 
    int counter;

public:
    CircularQueue() {
        f = 0;
        r = -1;
        counter = 0;
    }

    void enqueue(int num) {
        if (counter <= 0) {
            cout << "\nList is already full." << endl;
        } 
        r = (r + 1) % MAX;
        
    }
    
    void dequeue() {

    }

    void display() {

    }{file_extension}
};{file_extension}

struct Node {
    int data;
    Node* next;

    Node(int x) : data(x), next(NULL) {}
};

class LinkedListQueue {
private:
    Node* f; 
    Node* r; 

public:
    LinkedListQueue() {
        f = NULL;
        r = NULL;
    }

    void enqueue(int num) {
        Node* newNode = new Node(num);
        if (r == NULL) {
            f = r = newNode;
        } else {
            r->next = newNode;
            r = newNode;
        }
    }

    void dequeue() {
        if (f == NULL) {
            cout << "List is empty." << endl;
            return;
        }
        Node* temp = f;
        cout << "Removed: " << temp->data << endl;
        f = f->next;
        
        if (f == NULL) {
            r = NULL;
        }
        delete temp;
    }

    void display() {
        if (f == NULL) {
            cout << "\nCURRENT QUEUE: Empty\n";
            return;
        }
        cout << "\nCURRENT QUEUE: ";
        Node* temp = f;
        while (temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << "\n";
    }
};


int main() {

}


