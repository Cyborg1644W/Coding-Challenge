#include <stdio.h>
#include <math.h>

int main(){
    
    int UserNum;
    printf("Enter a Number: ");
    scanf("%d", &UserNum);

    printf("The Number of Digits is %.0f", floor(log10(UserNum))+1);
    return 0;
}