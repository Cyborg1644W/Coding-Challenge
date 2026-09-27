#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#define USERS 5
#define NAME_LEN 20

int accountNumbers[USERS] = {1001, 1002, 1003, 1004, 1005};
int pins[USERS] = {2131, 1433, 6732, 8012, 1273};
char names[USERS][NAME_LEN] = {"Alice", "Bob", "Charlie", "David", "Eve"};
float balances[USERS] = {1000.0, 500.0, 750.0, 1200.0, 300.0};
int current_user = -1;

void get_any(){
    printf("Enter Any Key to Continue...");
    getchar();
}

float getPositiveNumber() {
    float num;
    char c;

    while (scanf("%f%c", &num, &c) != 2 || c != '\n' || num <= 0) {
        printf("Invalid input\n");
        get_any();
        printf("\033[F\033[K");
        printf("\033[F\033[K");
        printf("\033[F\033[K");
        printf("Enter a positive number only: ");
    }
    return num;
}

void greet_user(){
    system("cls");
    printf("======================\n");
    printf("     HELLO %s\n",names[current_user]);
    printf("======================\n");
}

void show_menu(){   
    greet_user();
    printf("[1] Balance\n");
    printf("[2] Deposit\n");
    printf("[3] Withdraw\n");
    printf("[4] Transfer\n");
    printf("[5] Logout\n");    
}

int get_menu_choice(){
    show_menu();
    printf("Enter a positive number: ");
    int userChoice = getPositiveNumber();
    while (userChoice > 5){
        system("cls");
        show_menu();
        printf("1 - 5 Only\n");
        get_any();
        printf("\033[F\033[K");
        printf("\033[F\033[K");
        printf("Enter a positive number only: ");
        userChoice = getPositiveNumber();
    }
}

void get_balance(){
    system("cls");
    printf("=======================\n");
    printf("Balance    :   %.2f\n", balances[current_user]);
    printf("=======================\n");
    get_any();
}

void deposit_money(){
    printf("=======================\n");
    printf("        Deposit\n");
    printf("=======================\n");
    printf("Enter Amount : ");
    int amount = getPositiveNumber();  
    
}

void withdraw_money(){

}

void transfer_money(){

}

void logout(){
    
}

void introduction(){
    system("cls");
    printf("=======================\n");
    printf("      ATM MACHINE\n");
    printf("=======================\n");
}

void login(){
    int accNumber;
    int accPin;
    introduction();
    printf("Enter Account Number : ");
    accNumber = getPositiveNumber();
    printf("Enter Pin (4digit number) : ");
    accPin = getPositiveNumber();
    
    for (int i = 0; i <= USERS - 1; i++) {
        if(accountNumbers[i] == accNumber && pins[i] == accPin){
            current_user = i;
            break;
        }
    }
}

int main(){
    int choice = 0.0f;
    bool isRunning = true;
    
    login();
    while(isRunning){
        choice = get_menu_choice();
        switch(choice){
            case 1:
                get_balance();
                break;
            case 2:
                deposit_money();
                break;
            case 3:
                withdraw_money()          ;
                break;
            case 4:
                transfer_money();
                break;
            case 5:
                logout();
        }
    }
    return 0;
}