#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define RESET     "\033[0m"
#define YELLOW    "\033[0;33m"
#define ORANGE    "\033[38;2;255;140;0m"
#define CYAN      "\033[0;36m"
#define RED       "\033[0;31m"
#define GREEN     "\033[0;32m"
#define B_CYAN    "\033[1;36m"
#define B_RED     "\033[1;31m"

#define INDENT    "\t\t\t\t\t\t"
#define T_INDENT  "\t\t\t\t\t"

int showMenu();
void showTitle();
int getChoice(const char* prompt, int start, int end);
void clearBuffer();
void createArray(int array[]);
void sortArray();


int main() {
    int array[100];
    do {
        switch(showMenu()) {
            case 1:
                createArray(array);
                break;
            case 2:
                
        }

    } while ( );

    return 0;
}

int showMenu() {
    showTitle();
    printf(INDENT CYAN "Create Array" YELLOW "\t[1]\n");
    printf(INDENT CYAN "Sort Array" YELLOW "\t[2]\n");
    printf(INDENT CYAN "View Array" YELLOW "\t[3]\n");
    printf(INDENT CYAN "Delete Array" YELLOW "\t[4]\n");
    printf(INDENT CYAN "Exit" YELLOW "\t\t[5]\n");
    printf(RESET);
    
    return getChoice(INDENT ORANGE "Enter Your Choice: " RESET, 1, 5);
}

void showTitle() {
    printf(T_INDENT B_CYAN"==================================\n");
    printf(T_INDENT B_CYAN"         A   R   R   A   Y\n");
    printf(T_INDENT B_CYAN"==================================\n");
    printf(RESET);
} 

int getChoice(const char* prompt, int start, int end) {
    int num;
    while(true) {
        printf("%s", prompt);
        if(scanf("%d", &num) != 1) {
            printf(INDENT RED "[ERROR] " YELLOW "Invalid Character\n");
            clearBuffer();
        } 
        else if(num < start || num > end) {
            printf(INDENT RED "[ERROR] " YELLOW "Number Out of Range\n");
            clearBuffer();
        } else {
            clearBuffer();
            return num;
        }
    }
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void createArray(int array[]) {
    int size = getChoice("Enter Array Size: ",0 , 100);
    for(int i = 0; i < size; i++) {
        printf("Enter Num %d: ", i + 1);
        getChoice("",0,100);
    }
}

void sortArray() 