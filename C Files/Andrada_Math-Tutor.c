//IDE VS CODE
//OS : Window 11


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>


int showMenu(){
    int UserChoice=0;
    printf("===================\n");
    printf("     MATH TUTOR\n");
    printf("===================\n");
    printf("[1] Addition\n");
    printf("[2] Subtraction\n");
    printf("[3] Multiplication\n");
    printf("[4] Division\n");
    printf("[0] Exit\nEnter Here -> ");
    scanf("%d", &UserChoice);
    return UserChoice;
}


void showError(int choice){
    system("cls");
    printf("May Nakita ka ba dyan na %d?\n", choice);
    Sleep(3000);
    system("cls");
}


int generateRandom(){
    return (rand() % 100); 
}


void compareAnswer(int UserAnswer, int FinalAnswer, int *CorrectCount, int *TotalCount){
    (*TotalCount)++;
    if (UserAnswer == FinalAnswer){
        printf("CORRECT!\n");
        (*CorrectCount)++;
    } else{
        printf("%d Is The Correct Answer :(\n",FinalAnswer);
    }
    Sleep(700);
}


int add(int Augend, int Addend){
system("cls");
printf("What is %d + %d : ", Augend, Addend);
return Augend + Addend;
}


int subtract(int Minuend, int Subtrahend){
    system("cls");  
    printf("What is %d - %d : ", Minuend, Subtrahend);
    return Minuend - Subtrahend;
}


int multiplication(int Num1, int Num2){
    system("cls");  
    printf("What is %d x %d : ", Num1, Num2);
    return Num1 * Num2;
}


int division(int Dividend, int Divisor){
    system("cls");  
    printf("What is %d / %d(don't include remainder) : ", Dividend, Divisor);
    return Dividend / Divisor;
}


int getremainder(int Dividend, int Divisor){
    printf("What is the remainder of %d / %d : ", Dividend, Divisor);
    return Dividend % Divisor;
}


void showscore(int Correct, int Total){
    printf("Correct Answer: %d\n", Correct);
    printf("Wrong Answer: %d\n",Total - Correct);
}


void inputerror(){
    printf("Positive Integer only!\n");
}


int main() {
    srand(time(NULL));
    bool IsRunning = true;

    int UserChoice, UserAnswer, Answer;
    int a, b, minuend, subtrahend;
    int correct=0, total=0;  //score 

    while (IsRunning) {;
        showscore(correct, total);
        UserChoice = showMenu();
        UserAnswer = 0;
        Answer = 0;

        switch(UserChoice) {
            case 0:
                Sleep(500);
                system("cls");
                printf("Exiting Program...\n");
                Sleep(1000);
                system("cls");
                IsRunning = false;
                break;

            case 1:
                Answer = add(generateRandom(), generateRandom());
                scanf("%d", &UserAnswer);
                compareAnswer(UserAnswer, Answer, &correct, &total);
                break;

            case 2:
                a = generateRandom();
                b = generateRandom();
                minuend = (a > b) ? a : b;
                subtrahend = (a > b) ? b : a;
                Answer = subtract(minuend, subtrahend);     
                scanf("%d", &UserAnswer);
                compareAnswer(UserAnswer, Answer, &correct, &total);
                break;

            case 3:
                Answer = multiplication(generateRandom(), generateRandom());
                scanf("%d", &UserAnswer);
                compareAnswer(UserAnswer, Answer, &correct, &total);
                break;

            case 4:
                a = generateRandom();
                b = generateRandom();
                if (b == 0) b = 1;

                int userQuotient, userRemainder;
                int quotient = division(a, b);
                scanf("%d", &userQuotient);

                int remainder = getremainder(a, b);
                scanf("%d", &userRemainder);

                total++; //didn't use compare function para di 2 point yung division
                if (userQuotient == quotient && userRemainder == remainder) {
                    printf("CORRECT!\n");
                    correct++;
                } else {
                    printf("Correct Answer\n");
                    printf("Quotient: %d, Remainder: %d\n", quotient, remainder);
                }
                Sleep(700);
                break;

            default:
                showError(UserChoice);
                break;
        }
    }
    return 0;
}