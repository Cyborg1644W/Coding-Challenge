#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>


int main() {
    int Sec=0;
    printf("Countdown\n");
    printf("Enter A Number is Seconds : ");
    scanf("%d", &Sec);
    
    for (int i = Sec; i !=0; i--){
        printf("%d\n", i);
        Sleep(1000);
    }
    
    printf("Time's UP");
    return 0;
}