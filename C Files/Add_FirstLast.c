#include <stdio.h>
#include <math.h>

int main(){
    int UserNum;
    float FirstDigit, LastDigit, UserSum;
    printf("Enter a number :  ");
    scanf("%i", &UserNum);

    // log 10 () + 1   = number of digits
    // may loop rin pero mas efficient to sa memory and time
    FirstDigit = floor(UserNum / (pow(10,floor(log10(UserNum)))));

    LastDigit = UserNum % 10;
    UserSum = FirstDigit + LastDigit;
    printf("The First Digit Digit is %.0f\n", FirstDigit);
    printf("The Last Digit is %.0f\n", LastDigit);
    printf("The Sum of %.0f and %.0f is %.0f\n", FirstDigit, LastDigit, UserSum);

    return 0;
}