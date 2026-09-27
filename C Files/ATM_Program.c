#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

//500 maintaining balance
int main(){
    char AccountName[20];
    int UserChoice;
    float Balance = 500, Withdraw = 0.0f , Deposit = 0.0f;
    bool IsRunning;

    IsRunning = true;
    while (IsRunning) {
        printf("1 : Deposit money \n2 : Withdraw money \n3 : Check balance \n4 : Exit\n-> ");
        scanf("%d", &UserChoice);
        
        switch(UserChoice){
            case 1:
                printf("Depositing Money...\n");
                Sleep(700);
                printf("Enter Amount -> ");
                Sleep(700);
                scanf(" %f", &Deposit);
                Balance += Deposit;
                printf("Balance : %.2f\n", Balance);
                Sleep(700);
                break;

            case 2:
                printf("Withdrawing Money...\n");
                Sleep(700);
                printf("Enter Amount -> ");
                Sleep(700);
                scanf(" %f", &Withdraw);
                if (Withdraw > Balance){
                    printf("Insufficient Funds\n");
                    Sleep(700);
                    printf("Balance : %f\n", Balance);
                    Sleep(700);
                }
                else {
                    if (Balance - Withdraw < 500){
                        Sleep(700);
                        printf("Insuffcient Funds\n");
                        Sleep(700);
                        printf("The Maintaining Balance is 500\n");
                        Sleep(700);
                    }
                    else{
                        Balance -= Withdraw;
                    }
                    printf("Balance : %.2f\n", Balance);
                    Sleep(700);
                }
                break;

            case 3:
                Sleep(700);
                printf("Balance : %.2f\n", Balance);
                Sleep(700);
                break;

            case 4:
                printf("Exiting Account...");
                Sleep(700);
                IsRunning = false;
                break;

            default:
                printf("Invalid Input\n");
                Sleep(700);
        }
    }
}