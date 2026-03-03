#include <stdio.h>
#include <stdlib.h>

void getRowSum(int rows, int column, int array[][100]);
void makeArray(int rows, int column, int array[][100]);
void displayArray(int rows, int column, int array[][100]);
int getInteger(char *prompt);


int main() {
    int array[100][100];
    int column = getInteger("Enter Number of Column: ");
    int rows = getInteger("Enter Number of Rows: ");
    
    makeArray(rows, column, array);
    displayArray(rows, column, array);
    getRowSum(rows, column, array);
    
    return 0;
}

void getRowSum(int rows, int column, int array[][100]) {
    for(int i = 0; i < rows; i++) {
        int rowSum = 0;
        for(int j = 0; j < column; j++) {
            rowSum += array[i][j];
        }
        printf("Row %d Sum is %d\n", i+1, rowSum);
    }
}

void makeArray(int rows, int column, int array[][100]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < column; j++) {
            array[i][j] = getInteger("Enter a Number: ");
        }
    }
}

void displayArray(int rows, int column, int array[][100]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < column; j++) {
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
}

int getInteger(char *prompt) {
    int i;
    printf("%s", prompt);
    while (scanf("%d", &i) != 1) {
        printf("Invalid input!\n");
        printf("Input an integer: ");
        while (getchar() != '\n');
    }
    return i;
}