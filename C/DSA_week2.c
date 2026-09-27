#include <stdio.h>
#include <conio.h>

#define MAX 100

typedef struct list {
    int DATA[MAX];
    int last;
} LIST;

LIST L;

void makenull() {
    L.last = -1;
}

int isfull() {
    return (L.last == MAX - 1);
}

void insert(int x) {
    if (isfull()) {
        printf("List is full.");
        getch();
    } else {
        L.last++;
        L.DATA[L.last] = x;
    }
}

int locate(int x) {
    int i;
    for (i = 0; i <= L.last; i++)
        if (L.DATA[i] == x)
            return (i);
    return (-1);
}

int isempty() {
    return (L.last == -1);
}

void del(int x) {
    int p;
    if (isempty()) {
        printf("List is empty.");
        getch();
    } else {
        p = locate(x);
        if (p < 0) {
            printf("Not found.");
            getch();
        } else {
            for (int i = p; i < L.last; i++)
                L.DATA[i] = L.DATA[i + 1];
            L.last--;
        }
    }
}

void display() {
    int i;
    system("cls");
    printf("The list contains...\n");
    for (i = 0; i <= L.last; i++)
        printf("%d.) %d\n", i + 1, L.DATA[i]);
}

int main() {
    int choice, x;

    makenull();

    do {
        printf("\n===== ADT LIST MENU =====\n");
        printf("1. Add x\n");
        printf("2. Delete x\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value of x: ");
                scanf("%d", &x);
                insert(x);
                break;
            case 2:
                printf("Enter value of x: ");
                scanf("%d", &x);
                del(x);
                break;
            case 3:
                display();
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 4);

    return 0;
}