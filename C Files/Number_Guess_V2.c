#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int RandomNumber(int NumRange){
    srand(time(NULL));
    int num = (rand() % NumRange) + 1;
    return num;
}

int NumberGuess(){
    int NumRange;
    int Number, Userguess, Score = 100, Attempt = 0,MaxAttempt, difficulty=0;
    bool IsRunning = true;
    printf("Number Guessing Game\n");

    printf("Game Difficulty (1:Easy, 2:Medium, 3:Hard, 4:Extreme)\n-> ");
    scanf("%d", &difficulty);
    switch (difficulty){
        case 1:
            printf("Difficulty Set to Easy\n");
            MaxAttempt = 10;
            NumRange = 50;
            Number = RandomNumber(NumRange);
            break;
        case 2:
            printf("Difficulty Set to Medium\n");
            MaxAttempt = 7;
            NumRange = 100;
            Number = RandomNumber(NumRange);
            break;
        case 3:
            printf("Difficulty Set to Hard\n");
            MaxAttempt = 5;
            NumRange = 100;
            Number = RandomNumber(NumRange);
            break;
        case 4:
            printf("Difficulty Set to Extreme\n");
            MaxAttempt = 5;
            NumRange = 150;
            Number = RandomNumber(NumRange);
            break;
        default:
            printf("Invalid Input\nDifficulty Set to Medium\n");
            MaxAttempt = 7;
            NumRange = 100;
    }
    
    while (IsRunning){
        printf("Enter Your Guess (1-%d): ",NumRange);
        scanf("%d", &Userguess);
        
        if (Attempt == MaxAttempt) {
            printf("Game Over, Number was %d\n",Number);
            IsRunning = false;
        }

        if (Userguess < Number){
            printf("Higher!\n");
            Score -= 10;
            Attempt += 1;
        }
        else if (Userguess > Number){
            printf("Lower!\n");
            Score -= 10;
            Attempt ++;
        }
        else {
            printf("Correct\n");
            printf("Score : %d\n", Score);
            printf("Attempt/s : %d\n", Attempt);
            IsRunning = false;
        }
    }
}

int main(){
    NumberGuess();
    return 0;
}