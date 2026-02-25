#include <stdio.h>

void DiagonalSum(int array[][50], int size) {
    int totalSum = 0;
    for(int i = 0; i < size; i++) {
        totalSum += array[i][i];
        totalSum += array[i][(size-1)-i];
    }
    if((size & 1) == 1) {
        totalSum -= array[(size-1)/2][(size-1)/2];
    }
    printf("%d", totalSum);
}

int main() {
    int grid[50][50] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
    int size = 3;

    DiagonalSum(grid, size);
    return 0;
}