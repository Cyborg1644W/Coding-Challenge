#include <stdio.h>
#include <stdbool.h>

int main() {
    int UserNum;
    printf("Enter a 2-Digit Number: ");

    if (scanf("%d", &UserNum) != 1) {
        printf("You can only enter a number!\n");
    } 
    else if (UserNum < 1 || UserNum > 99) {
        printf("1 - 99 only!\n");
    } 
    else {
        printf("You entered: %d\n", UserNum);
    }

    
    return 0;
}