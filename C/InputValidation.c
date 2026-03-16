#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Function Prototypes 
void getStringNoNumbers(const char* prompt, char* buffer, int size);
int getPositiveInt(const char* prompt);
int getNegativeInt(const char* prompt);
double getDouble(const char* prompt);
void clearBuffer();

int main() {
    char name[100];
    getStringNoNumbers("Enter your name: ", name, sizeof(name));
    
    int age = getPositiveInt("Enter your age: ");
    int debt = getNegativeInt("Enter your debt: ");
    double gpa = getDouble("Enter your GPA: ");

    printf("\nResults:\n");
    printf("Name: %s\n", name);
    printf("Age: %d\n", age);
    printf("Debt: %d\n", debt);
    printf("GPA: %.2f\n", gpa);

    return 0;
}

// --- Function Definitions ---

// Helper to clear stdin (equivalent to cin.ignore)
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// String input (letters only)
void getStringNoNumbers(const char* prompt, char* buffer, int size) {
    while (true) {
        printf("%s", prompt);
        fgets(buffer, size, stdin);

        // Remove trailing newline from fgets
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        bool valid = true;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isdigit(buffer[i])) {
                valid = false;
                break;
            }
        }

        if (valid) return;
        printf("Letters only. Try again.\n");
    }
}

// Positive integer
int getPositiveInt(const char* prompt) {
    int x;
    while (true) {
        printf("%s", prompt);
        if (scanf("%d", &x) != 1 || x <= 0) {
            printf("Enter a positive integer.\n");
            clearBuffer(); // Clean up bad input
        } else {
            clearBuffer(); // Clean up the newline
            return x;
        }
    }
}

// Negative integer
int getNegativeInt(const char* prompt) {
    int x;
    while (true) {
        printf("%s", prompt);
        if (scanf("%d", &x) != 1 || x >= 0) {
            printf("Enter a negative integer.\n");
            clearBuffer();
        } else {
            clearBuffer();
            return x;
        }
    }
}

// Double input
double getDouble(const char* prompt) {
    double x;
    while (true) {
        printf("%s", prompt);
        if (scanf("%lf", &x) != 1) {
            printf("Enter a number.\n");
            clearBuffer();
        } else {
            clearBuffer();
            return x;
        }
    }
}