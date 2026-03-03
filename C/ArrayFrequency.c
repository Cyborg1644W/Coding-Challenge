#include <stdio.h>

void makeArray(int rows, int column, int array[][100]);
void displayArray(int rows, int column, int array[][100]);
void getCornerSum(int rows, int column, int array[][100]);
int getInteger(char *prompt);

int main() {
    int rows = getInteger("Enter Number of Rows: ");
    int column = getInteger("Enter Number of Column: ");
    int numArray[100][100];

    makeArray(rows, column, numArray);
    displayArray(rows, column, numArray);
    getCornerSum(rows, column, numArray);
    return 0;
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


void makeArray(int rows, int column, int array[][100]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < column; j++) {
            array[i][j] = getInteger("Enter a Number: ");
        }
    }
}

void displayArray(int rows, int column, int array[][100]) {
    printf("\n");
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < column; j++) {
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void getCornerSum(int rows, int column, int array[][100]) {
    int sum = 0;
    for(int i = 0; i < rows; i++) {
        if(i == 0 || i == rows -1) {
            for(int j = 0; j < column; j++) {
                sum += array[i][j];
            }
        } else {
            sum += array[i][0];
            sum += array[i][column - 1];
        }
    }
    printf("The Total Corner Sum is %d\n\n", sum);
}
