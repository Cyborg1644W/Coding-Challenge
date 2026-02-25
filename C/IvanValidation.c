#include <stdio.h>
#include <stdlib.h>

void clear() {
    system("cls");
}

void integerValidation() {
    clear();
    int i;
    printf("Input an integer: ");
    while (scanf("%d", &i) != 1) {
        printf("Invalid input!\n");
        printf("Input an integer: ");
        while (getchar() != '\n');
    }
    printf("Integer: %d\n", i);
}

void charValidation() {
    /*
        switch case nalang kasi halos lahat sa kb char naman
    */
}

int main () {

    char choice;

    while (1) {
        printf("1: Integer\n2: Char\n'e': Exit\nChoice: ");
        scanf("%c", &choice);
        switch (choice){
            case '1':
                integerValidation();
                break;
            case '2':
                charValidation();
                break;
            case 'e':
                exit(0);
            default: printf("invalid input!\n");
        }
    }
    
    return 0;
}