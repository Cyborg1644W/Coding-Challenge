#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>


int ShowMenu(){
    int UserChoice=0;
    printf("\n\n1 : Palindrome Checker\n2 : Prime Number Checker\n3 : Fibonacci Generator\n4 : Factorial\n5 : Sum of Digits\n");
    printf("6 : Prime and Composite Separator\n7 : Triangle Pattern \n8 : Exit Program\n-> ");
    scanf("%d", &UserChoice); 
    return UserChoice;
}


void Palindrome(){
    int num, reversed = 0, remainder, original;
    printf("Enter an integer: ");
    scanf("%d", &num);
    
    original = num;

    while (num != 0){
        remainder = num % 10;
        reversed = reversed * 10 + remainder;
        num /= 10;
    }

    if (original == reversed) {
        printf("%d is a palindrome.", original);
    } else {
        printf("%d is not a palindrome.", original);
    }
    Sleep(900);
}


void PrimeNumber(){
    int UserNumber=0;
    bool IsNotPrime = true;
    printf("Enter a Number: ");
    scanf("%d", &UserNumber);

    for(int i = 2; i <= sqrt(UserNumber); i++){
        if (UserNumber % i ==0 && UserNumber != i){
            IsNotPrime = false;
            break;
        }
    }
    if (IsNotPrime){
        if (UserNumber == 2){
            printf("%d is a prime number", UserNumber);
        } else {
            printf("%d is NOT a prime number",UserNumber);
        }
    }
    else{
        printf("%d is NOT a prime number",UserNumber);
    }
    Sleep(1000);
}


void Fibonacci(){
    int UserNumber=0, result=0;
    long long a=0, b=1;
    printf("Enter the Nth term : ");
    scanf("%d", &UserNumber);
    for(int i=1; i<=UserNumber-1; i++){
        b = a + b, a = b - a;
    }
    printf("the result is : %d", b);
    Sleep(900);
}


void Factorial(){
    int UserNum=0, result=1;
    printf("Enter a Number: ");
    scanf("%d", &UserNum);

    for (int i=UserNum; i>=1; i--){
        result *= i;
    }
    printf("%d Factorial is %d", UserNum, result);
    Sleep(900);   
}


void SumOfDigits(){
    int UserNum=0, Sum=0, digit=0, original;
    printf("Enter a Number: ");
    scanf("%d", &UserNum);
    original = UserNum;
    while(UserNum!=0){
        digit = UserNum % 10;
        Sum += digit;
        UserNum /= 10;
    }
    printf("The Sum of the Digits of %d is %d", original, Sum);
    Sleep(900);
}


void PrimeSeparator() {
    int UserNum = 0;
    int Primes[100], NonPrimes[100];
    int PrimesCount = 0, NonPrimesCount = 0;

    printf("Enter a Number : ");
    scanf("%d", &UserNum);

    for (int CurrentNum = 2; CurrentNum <= UserNum; CurrentNum++) {
        int is_prime = 1;

        for (int j = 2; j <= sqrt(CurrentNum); j++) {
            if (CurrentNum % j == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            Primes[PrimesCount] = CurrentNum;
            PrimesCount++;
        } else {
            NonPrimes[NonPrimesCount] = CurrentNum;
            NonPrimesCount++;
        }
    }
    printf("Composite up to %d : ", UserNum);
    for (int i = 0; i < NonPrimesCount; i++) {
        if (i == NonPrimesCount -1){
            printf("%d", NonPrimes[i]);
            break;
        }
        printf("%d, ", NonPrimes[i]);
    }

    printf("\n\nPrimes up to %d : ", UserNum);
    for (int i = 0; i < PrimesCount; i++) {
        if (i == PrimesCount -1){
            printf("%d", Primes[i]);
            break;
        }
        printf("%d, ", Primes[i]);
    }
    Sleep(1000);
}


void PatternPrint(){
    int UserNum=0;
    char Userchar;
    printf("Enter how big the triangle is : ");
    scanf("%d", &UserNum);
    printf("What character do you want (1 character only): ");
    scanf(" %c", &Userchar);
    
    
    for (int j =  1; j <= UserNum; j++){
        for (int i = 1; i<= j; i++){
            printf(" %c ", Userchar);
        }
        printf("\n");
    }
}


void ShowError(int UserInput){
    printf("INVALID INPUT\n");
    Sleep(900);
    printf("May nakita ka bang %d dyan??", UserInput);
    Sleep(1300);
}


int main(){
    bool IsRunning = true;
    while(IsRunning){
        int choice = ShowMenu();
        switch(choice){
            case 1:
                Palindrome();
                break;
            case 2:
                PrimeNumber();
                break;
            case 3:
                Fibonacci();
                break;
            case 4:
                Factorial();
                break;
            case 5:
                SumOfDigits();
                break;
            case 6:
                PrimeSeparator();
                break;
            case 7:
                PatternPrint();
                break;
            case 8:
                printf("Exiting Program...");
                IsRunning = false;
                Sleep(1300);
                break;
            default:
                ShowError(choice);
        }
    }
    return 0;
}