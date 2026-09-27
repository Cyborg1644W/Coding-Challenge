#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>


float CorrectAnswer=0;
float TotalQuestion=0;
int maxnum=0;

int generate_random(){
    return (rand() % 100); 
}


void show_score(){
    if (TotalQuestion != 0){
        printf("Score: %.2f\n", CorrectAnswer);
        printf("Accuracy: %.2f%%\n", (CorrectAnswer/ TotalQuestion)*100);
    }
}


int show_menu(){
    int userchoice = 0;
    printf("\n===================\n");
    printf("     MATH TUTOR\n");
    printf("===================\n");
    printf("[1] Addition\n");
    printf("[2] Subtraction\n");
    printf("[3] Multiplication\n");
    printf("[4] Division\n");
    printf("[0] Exit\nEnter Here -> ");

    if (scanf("%d", &userchoice) != 1){
        return -1;
    }
    return userchoice;
}


void get_difference(){
    system("cls");
    int UserAnswer = 0;
    int a = generate_random();
    int b = generate_random();
    int minuend = (a > b) ? a : b;
    int subtrahend = (a > b) ? b : a;
    printf("%d - %d = ", minuend, subtrahend);
    scanf("%d", &UserAnswer);
    int Answer = minuend - subtrahend;
    if (UserAnswer == Answer){
        printf("Correct!!\n");
        CorrectAnswer++;
    } else {
        printf("The correct answer is %d\n", Answer);
    }
    TotalQuestion++;
    Sleep(900);
}


void get_sum(){
    system("cls");
    int UserAnswer = 0;
    int augend = generate_random();
    int addend = generate_random();
    printf("%d + %d = ", augend, addend);
    scanf("%d", &UserAnswer);
    int Answer = augend + addend;
    if (UserAnswer == Answer){
        printf("Correct!!\n");
        CorrectAnswer++;
    } else {
        printf("The correct answer is %d\n", Answer);
    }
    TotalQuestion++;
    Sleep(900);
}


void multiply_number(){
    system("cls");
    int UserAnswer = 0;
    int factor1 = generate_random();
    int factor2 = generate_random();
    printf("%d x %d = ", factor1, factor2);
    scanf("%d", &UserAnswer);
    int Answer = factor1 * factor2;
    if (UserAnswer == Answer){
        printf("Correct!!\n");
        CorrectAnswer++;
    } else {
        printf("The correct answer is %d\n", Answer);
    }
    TotalQuestion++;
    Sleep(900);
}


void divide_number(){
    system("cls");
    int UserAnswer = 0;
    int a = generate_random();
    int b = generate_random();
    int dividend = (a > b) ? a : b;
    int divisor = (a > b) ? b : a;
    printf("%d / %d = ", dividend, divisor);
    scanf("%d", &UserAnswer);
    int Answer = dividend / divisor;
    if (UserAnswer == Answer){
        printf("Correct!!\n");
        CorrectAnswer++;
    } else {
        printf("The correct answer is %d\n", Answer);
    }
    TotalQuestion++;
    Sleep(900);
}


void show_error(int input){
    system("cls");
    Sleep(200);
    printf("%d is not in the menu, Enter a Again..\n", input);
    Sleep(900);
}


int main(){
    printf("How many math problem you want to answer: ");
    scanf("%d", &maxnum);
    system("cls");

    srand(time(NULL));
    bool Is_running = true;
    
    while(Is_running){
        if (TotalQuestion == maxnum){
            system("cls");
            show_score();
            break;
        }
        show_score();
        int choice = show_menu();
        switch(choice){
            case -1:
                printf("Invalid Input\n");
                Sleep(900);
                Is_running = false;
                system("cls");
                printf("Run the program Again");
                Sleep(700);
                break;
            case 1:
                get_sum();
                break;
            case 2:
                get_difference();
                break;
            case 3:
                multiply_number();
                break;
            case 4:
                divide_number();
                break;
            case 0:
                Is_running = false;
                system("cls");
                show_score();   
                printf("Exiting The Program...");
                Sleep(1200);
                system("cls");
                break;
            default:
                show_error(choice);
                break;
        }
    }
    return 0;
}