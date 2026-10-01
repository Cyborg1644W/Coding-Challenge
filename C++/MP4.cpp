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
        if (counter >= MAX) {
            cout << "List is already full." << endl;
            return;
        }
        r = (r + 1) % MAX;
        myArr[r] = num;
        counter++;
    }
    
    void dequeue() {
        if (counter <= 0) {
            cout << "List is empty." << endl;
            return;
        }
        cout << "Removed: " << myArr[f] << endl;
        f = (f + 1) % MAX;
        counter--;
    }

    void display() {
        if (counter == 0) {
            cout << "\nCURRENT QUEUE: Empty\n";
            return;
        }
        cout << "\nCURRENT QUEUE: ";
        int index = f;
        for(int i = 0; i < counter; i++) {
            cout << myArr[index] << " ";
            index = (index + 1) % MAX;
        }
        cout << "\n";
    }
};

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

int mainMenu() {
    int choice;
    cout << "\nMenu\n";
    cout << "1 Circular Queue\n";
    cout << "2 Link List Queue\n";
    cout << "3 Exit\n";
    cout << "Enter choice: ";
    cin >> choice;
    return choice;
}

int main() {
    int num, userinput;
    
    while(1) {

        switch (mainMenu()) { 
            case 1: {
                CircularQueue cq; 
                while(1) {
                    cout << "\nQueue Operations\n";
                    cout << "1 Enqueue\n2 Dequeue\n3 Display\n4 Exit\n";
                    cout << "Enter choice: ";
                    cin >> num; 
                    
                    if (num == 4) break; 
                    
                    switch (num) {
                        case 1: 
                            cout << "Enter value: ";
                            cin >> userinput;
                            cq.enqueue(userinput);
                            break;
                        case 2:
                            cq.dequeue();
                            break;
                        case 3:
                            cq.display();
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                }
                break; 
            }
            case 2: {
                LinkedListQueue llq;
                while(1) {
                    cout << "\nQueue Operations\n";
                    cout << "1 Enqueue\n2 Dequeue\n3 Display\n4 Exit\n";
                    cout << "Enter choice: ";
                    cin >> num; 
                    
                    if (num == 4) break; 
                    
                    switch (num) {
                        case 1: 
                            cout << "Enter value: ";
                            cin >> userinput;
                            llq.enqueue(userinput);
                            break;
                        case 2:
                            llq.dequeue();
                            break;
                        case 3:
                            llq.display();
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                }
                break; 
            }
            case 3:
                return 0; 
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}