#include <stdio.h>
#include <stdlib.h>


float get_positive(){
    float num;
    char c;

    printf("Enter a positive number: ");
    while (scanf("%f%c", &num, &c) != 2 || c != '\n' || num <= 0) {
        while (getchar() != '\n');
        printf("Invalid input. Enter a positive number only: ");
    }
    return num;
}



int main(){

        
    return 0;
}