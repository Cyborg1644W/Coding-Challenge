#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int x){
        data = x;
        left = NULL;
        right = NULL;
    }
};

struct QueueNode {
    Node* treeNode;
    QueueNode* next;
    
    QueueNode(Node* node) {
        treeNode = node;
        next = NULL;
    }
};

class MyQueue {
private:
    QueueNode *head, *tail;
public:
    MyQueue() { 
        head = NULL;
        tail = NULL; 
    }

    bool isEmpty() { 
        return head == NULL; 
    }

    void enqueue(Node* node) {
        QueueNode* temp = new QueueNode(node);
        if (tail == NULL) { 
            head = tail = temp; 
            return; 
        }
        tail->next = temp;
        tail = temp;
    }

    Node* dequeue() {
        if (isEmpty()) return NULL;

        QueueNode* temp = head;
        Node* retrievedNode = temp->treeNode;
        head = head->next;

        if (head == NULL) {
            tail = NULL;
        }
        delete temp;
        return retrievedNode;
    }
};

class Btree{
public:
    Btree(){} 

    void inorder(Node* currentNode){
        if (currentNode == NULL) return;
        inorder(currentNode->left);
        cout << currentNode->data << " ";
        inorder(currentNode->right);
    }
    
    void preorder(Node* currentNode){
        if (currentNode == NULL) return;
        cout << currentNode->data << " ";
        preorder(currentNode->left);
        preorder(currentNode->right);
    }
    
    void postorder(Node* currentNode){
        if (currentNode == NULL) return;
        postorder(currentNode->left);
        postorder(currentNode->right);
        cout << currentNode->data << " ";
    }

    void levelorder(Node* currentNode){
        if (currentNode == NULL) return;
        
        MyQueue q;
        q.enqueue(currentNode); 
        
        while (!q.isEmpty()) {
            Node* temp = q.dequeue();
            cout << temp->data << " ";
            
            if (temp->left != NULL) q.enqueue(temp->left);
            if (temp->right != NULL) q.enqueue(temp->right);
        }
    }

    Node* buildTree(){
        int data;
        cin >> data;
        
        if (data == -1) {
            return NULL;
        }
        
        Node* temp = new Node(data);
        
        cout << "Enter left child of " << data << " (-1 for NULL): ";
        temp->left = buildTree();
        
        cout << "Enter right child of " << data << " (-1 for NULL): ";
        temp->right = buildTree();
        
        return temp;
    }
};

int main(){
    Btree bt;
    Node* tree = NULL; 
    int choice;

    do {
        cout << "\n\nMenu\n";
        cout << "1. Inorder\n";
        cout << "2. Preorder\n";
        cout << "3. Postorder\n";
        cout << "4. Level order\n";
        cout << "5. Build Tree\n";
        cout << "6. exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        cout << "\n"; 

        switch(choice){
            case 1:
                cout << "Inorder: ";
                if (tree == NULL) cout << "Tree is empty!";
                else bt.inorder(tree);
                cout << "\n";
                break;
            case 2:
                cout << "Preorder: ";
                if (tree == NULL) cout << "Tree is empty!";
                else bt.preorder(tree);
                cout << "\n";
                break;
            case 3:
                cout << "Postorder: ";
                if (tree == NULL) cout << "Tree is empty!";
                else bt.postorder(tree);
                cout << "\n";
                break;
            case 4:
                cout << "Level order: ";
                if (tree == NULL) cout << "Tree is empty!";
                else bt.levelorder(tree);
                cout << "\n";
                break;
            case 5:
                cout << "Enter root node (-1 for NULL): ";
                tree = bt.buildTree();
                cout << "\nTree built successfully!\n";
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 6);

    return 0;
}