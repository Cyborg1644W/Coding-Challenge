// Language: C
#include <stdio.h>

// Function to print a pyramid of given height
void printPyramid(int height) {
    for (int i = 1; i <= height; i++) {
        // Print leading spaces
        for (int j = i; j < height; j++) {
            printf(" ");
        }
        // Print pyramid characters (*)
        for (int k = 1; k <= (2*i - 1); k++) {
            printf("*");
        }
        // Move to the next line
        printf("\n");
    }
}

// Usage example
int main() {
    int rows;
    printf("Enter the number of rows for the pyramid: ");
    scanf("%d", &rows);
    printPyramid(rows);
    return 0;
}