#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

// rectangle
// triangle
// parallelogram
// trapezoid : ((a+b)h)/2 

int showMenu(){
    int UserChoice=0;
    printf("1 : rectangle\n2 : triangle\n3 : parallelogram\n4 : trapezoid \n5 : Exit\n-> ");
    scanf("%d", &UserChoice);
    return UserChoice;
}

void rectangle(){
    float base=0.0f, height=0.0f;
    printf("Enter Base : ");
    scanf("%f", &base);
    printf("Enter Height : ");
    scanf("%f", &height);

    printf("The Area is %.2f\n", base*height);
    Sleep(1000);
}

void triangle(){
    float base=0.0f, height=0.0f;
    printf("Enter Base : ");
    scanf("%f", &base);
    printf("Enter Height : ");
    scanf("%f", &height);

    printf("The Area is %.2f\n", (base*height)/2);
    Sleep(1000);
}

void parallelogram(){
    float base=0.0f, height=0.0f;
    printf("Enter Base : ");
    scanf("%f", &base);
    printf("Enter Height : ");
    scanf("%f", &height);

    printf("The Area is %.2f\n", base*height);
    Sleep(1000);
}

void trapezoid(){
    float base1=0.0f, base2=0.0f, height=0.0f;
    printf("Enter top base : ");
    scanf("%f", &base1);
    printf("Enter bottom base : ");
    scanf("%f", &base2);
    printf("Enter Height : ");
    scanf("%f", &height);

    printf("The Area is %.2f\n", (height*(base1 + base2))/2); 
    Sleep(1000);

}

void ShowError(int number){
    printf("MAY NAKITA KA BANG %d DYAN!?\n", number);
    Sleep(1000);
}

int main(){
    bool IsRunning = true;
    while (IsRunning){
        int userchoice = showMenu(); 
        switch(userchoice){
            case 1:
                rectangle();
                break;
            case 2:
                triangle();
                break;
            case 3:
                parallelogram();
                break;
            case 4:
                trapezoid();
                break;
            case 5:
                IsRunning = false;
                printf("Exiting Program...");
                Sleep(1200);
                break;
            default:
                ShowError(userchoice);
        }

    }
    return 0;
}