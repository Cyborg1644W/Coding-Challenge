#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int RandomNumber(){
    srand(time(NULL));
    int num = (rand() % 100) + 1;
    return num;
}

int GameSystem(){
    int Number = RandomNumber(), Userguess, Score = 100, Attempt = 0;
    bool IsRunning = true;
    printf("Number Guessing Game \n");

    while (IsRunning){
        printf("Enter Your Guess : ");
        scanf("%d", &Userguess);
        
        if (Attempt == 7) {
            printf("YOU LOSE!\n");
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
    GameSystem();
    return 0;
}
