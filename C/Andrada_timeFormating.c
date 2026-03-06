#include <stdio.h>
#include <stdbool.h>

#define RESET     "\033[0m"
#define YELLOW    "\033[0;33m"
#define CYAN      "\033[0;36m"
#define GREEN   "\033[0;32m"
#define B_RED     "\033[1;31m"

void clearBuffer();
int secToHours(long time, int* hours, int* minutes, int* seconds, bool* isPM);
int getPositiveInt(const char* prompt);

int main() {
    int userTime;
    int status;
    bool isPM;
    int hours = 0 , minutes = 0, seconds = 0;
    do {
        userTime = getPositiveInt("Enter total duration in seconds: ");
        status = secToHours(userTime, &hours, &minutes, &seconds, &isPM);
        if (status == 1){
            printf(B_RED "[ERROR] " YELLOW"The Number Exceeds the Limit. " RESET"\n");
        }
    } while (status == 1);
    printf("\nTIME:");
    printf(GREEN "\n%02d" RESET ":" GREEN "%02d" RESET ":" GREEN "%02d" RESET " %s\n\n\n",
         hours, minutes, seconds, isPM ? "PM" : "AM");
 
    return 0;
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int secToHours(long time, int* hours, int* minutes, int* seconds, bool* IsPM) {
    if (time > 86400 || time <= 0) return 1;

    *hours = time / 3600;
    *minutes = (time % 3600) / 60;
    *seconds = time % 60;

    if (*hours >= 12) {
        *IsPM = true;
        if (*hours > 12) *hours -= 12; //for noon
    } else {
        *IsPM = false;
        if (*hours == 0) *hours = 12;  // for midnight
    }

    return 0; // Success
}
int getPositiveInt(const char* prompt) {
    int x;
    while (true) {
        printf(CYAN "%s"RESET, prompt);
        if (scanf("%d", &x) != 1 || x <= 0) {
            printf(B_RED "[ERROR] " YELLOW"A POSITIVE integer is required. " RESET".\n");
            clearBuffer(); 
        } else {
            clearBuffer(); 
            return x;
        }
    }
}
