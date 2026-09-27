#include <stdio.h>
#include <math.h>


int main(){
    float SideA, SideB, SideC, Discriminant;

    printf("Discriminant Calculator\n");
    printf("Enter Side A: ");
    scanf("%f", &SideA);
    printf("Enter Side B: ");
    scanf("%f", &SideB);
    printf("Enter Side C: ");
    scanf("%f", &SideC);

    Discriminant = pow(SideB, 2) - (4 * SideA * SideC);
 
    if (Discriminant > 0){
        printf("The Discriminant is %.2f\nThere are 2 real roots", Discriminant);
    }
    else if(Discriminant == 0){
        printf("The Discriminant is %.2f\nThere are 1 real root", Discriminant);
    }
    else if(Discriminant < 0){
        printf("The Discriminant is %.2f\nThere are 2 complex roots", Discriminant);
    }
}