#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(){
    int usertime=0;
    printf("COUNTDOWN CLOCK\n");
    printf("Enter How long(seconds) : ");
    scanf("%d", &usertime);

    while (usertime>=1){
        int hours = usertime/3600;
        int minutes = (usertime/60)%60;
        int seconds = usertime%60;
        system("cls");
        printf("%02d:%02d:%02d\n", hours,minutes,seconds);
        Sleep(1000);
        usertime--;
    }
    system("cls");
    printf("Time's Up");
    return 0;
}