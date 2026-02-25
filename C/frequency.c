#include <stdio.h>

void getFrequency(int, int[]);
void displayArray(int, int[]);
void sortArray(int, int[]);


int main() {
    int array[] = {1, 5, 2, 2, 2, 5, 3, 12, 11, 14, 16};
    int size = sizeof(array) / sizeof(array[0]);
    sortArray(size, array);
    displayArray(size, array);
    getFrequency(size, array);
    return 0;
}

void sortArray(int size, int a[]) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

void getFrequency(int size, int a[]) {
    int counter = 1;
    int frequent = 1;
    int highest = 0;
    //array
    for (int i = 0; i < size - 1; i++) {
        
        if (a[i] == a[i+1]) {
            printf("number %d repeats\n", a[i]);
            counter++;
            if (highest < counter) 
                frequent = a[i];
                highest = counter;
        } else if (a[i] != a[i + 1]) {
            printf("freq of no. %d is %d\n", a[i], counter);
            counter = 1;
        }
    }
    printf("Most frequent number is %d", frequent);
}

void displayArray(int size, int a[]) {
    printf("\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t", a[i]);
    }
    printf("\n");
}