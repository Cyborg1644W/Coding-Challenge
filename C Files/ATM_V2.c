#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

int LogIn(int AccountPassword, bool IsRunning){
    char AccountName[30];
    int Password, WrongPasswordCount=0;

    printf("WELCOME TO BDO\n");
    printf("Enter Your Name : ");
    scanf("%s", &AccountName);
    Sleep(700);
    printf("Hello, %s\n", AccountName);
    Sleep(700);
    
    while (true){
        if (WrongPasswordCount == 3){
            printf("Account Terminated for the next 24 Hours\n");
            Sleep(700);
            printf("Exiting...\n");
            Sleep(1000);
            IsRunning = false;
            break;
        }
        printf("Enter Your Password : ", AccountName);
        scanf("%d", &Password);

        if (AccountPassword != Password){
            Sleep(500);
            printf("Incorrect Password\n");
            Sleep(700);
            printf("Note: 3 wrong attempts, your ATM access will be suspended for the next 24 hours.\n");
            Sleep(700);
            WrongPasswordCount ++;
            printf("%d Wrong Attempts Before Suspention\n",3 - WrongPasswordCount);
            Sleep(1000);
        }

        else {
            Sleep(600);
            printf("Logging In...\n");
            Sleep(1000);
            printf("WElCOME %s\n", AccountName);
            break;
        }
    }
}


int ShowMenu(){
    int UserChoice;
    printf("1 : Deposit money \n2 : Withdraw money \n3 : Check balance \n4 : Exit\n-> ");
    scanf("%d", &UserChoice);
    return UserChoice;
}


float Deposit(float Balance){
    float Amount = 0.0f; 
    printf("Depositing Money...\n");
    Sleep(700);
    printf("Enter Amount -> ");
    Sleep(700);
    scanf("%f", &Amount);
    if (Amount < 0){
        printf("Invalid Amount\n");
        Sleep(700);
        printf("Balance : %.2f\n", Balance);
        Sleep(700);
    }
    else {
        Balance += Amount;
        printf("Balance : %.2f\n", Balance);
        Sleep(700);
    }
    return Balance;
}


void GetBalance(float Balance){
    Sleep(700);
    printf("Balance : %.2f\n", Balance);
    Sleep(700);
}


float Withdraw(float Balance){
    float Amount = 0.0f;
    printf("Withdrawing Money...\n");
    Sleep(700);
    printf("Enter Amount -> ");
    Sleep(700);
    scanf(" %f", &Amount);
    if (Amount > Balance){
        printf("Insufficient Funds\n");
        Sleep(700);
        printf("Balance : %f\n", Balance);
        Sleep(700);
    }
    else {
        if (Balance - Amount < 500){
            Sleep(700);
            printf("Insuffcient Funds\n");
            Sleep(700);
            printf("The Maintaining Balance is 500\n");
            Sleep(700);
        }
        else{
            Balance -= Amount;
        }
        printf("Balance : %.2f\n", Balance);
        Sleep(700);
    }
    return Balance;
}


int main(){
    const int AccountPassword = 2025;
    float Balance = 500;
    bool IsRunning = true;

    IsRunning = LogIn(AccountPassword, IsRunning);


    while (IsRunning){
        switch(ShowMenu()){
            case 1: 
                Balance = Deposit(Balance);
                break;
            case 2:
                Balance = Withdraw(Balance);
                break;
            case 3:
                GetBalance(Balance);
                break;
            case 4:
                printf("Exiting Account...");
                Sleep(700);
                IsRunning = false;
                break;
            default:
                printf("Invalid Input\n");
                Sleep(1000);
        }
    }
    return 0;
}