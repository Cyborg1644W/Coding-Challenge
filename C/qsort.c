#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b);
void makeArray(int size, int array[]);
void displayArray(int size, int array[]);
int getInteger(char* prompt);


int main() {
    int size = getInteger("Enter Array Size: "); 
    int numArray[100];
    makeArray(size, numArray);
    qsort(numArray, size, sizeof(int), compare);
    displayArray(size, numArray);
    return 0;
}

int compare(const void* a, const void* b) {
    return(*(int*)a - *(int*)b);
} 


void makeArray(int size, int array[]) {
    for(int i = 0; i < size ; i++) {
        array[i] = getInteger("Enter a Number");
    }
}

void displayArray(int size, int array[]) {
    for(int i = 0; i < size; i++) {
        printf("%d  ", array[i]);
    }
}

int getInteger(char* prompt) {
    int i;
    printf("%s", prompt);
    while (scanf("%d", &i) != 1) {
        printf("Invalid input!\n");
        printf("Input an integer: ");
        while (getchar() != '\n');
    }
    return i;
}
