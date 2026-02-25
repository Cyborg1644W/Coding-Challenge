#include <stdio.h>
#include <stdlib.h>

void sortArray(int size, int array[]);
int getInteger(char* prompt);
void makeArray(int size, int array[]);
void displayArray(int size, int array[]);
int compare(const void* a, const void* b);


int main() {
    int size = getInteger("Enter Array Size: ");
    int numArray[100];

    makeArray(size, numArray);
    displayArray(size, numArray);
    qsort(numArray, size, sizeof(int), compare);
    displayArray(size, numArray);

    return 0;
}

void sortArray(int size, int array[]) {
    for(int i = 0; i < size - 1 ; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(array[j] > array[j+1]) {
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
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

void makeArray(int size, int array[]) {
    for(int i = 0; i < size; i++) {
        array[i] = getInteger("Enter a Number: ");
    }
}

void displayArray(int size, int array[]) {
    printf("\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t", array[i]);
    }
    printf("\n");
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}