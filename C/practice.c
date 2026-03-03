#include <stdio.h>
#include <stdlib.h>

int arr[100];
int n;          

void inputValues();
void displayFrequency();

int main() {
    printf("Enter size: ");
    scanf("%d", &n);

    inputValues();
    displayFrequency();

    return 0;
}

void inputValues() {
    printf("Enter values:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
}

void displayFrequency() {
    int counted[100] = {0};  // list yung counted
    printf("\nFrequency:\n");

    for (int i = 0; i < n; i++) {

        // skip if already counted
        if (counted[i] == 1)
            continue;

        int count = 1;

        // count occurrences
        for (int j = i + 1; j < n; j++) {
            if (arr[j] == arr[i]) {
                count++;
                counted[j] = 1;   // mark as counted
            }
        }

        printf("%d - %d\n", arr[i], count);
    }
}
