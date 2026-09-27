#include <stdio.h>
#include <stdlib.h>

int main(){
    float Charge=0,Days=0; 
    printf("Car Rental Services\n");
    printf("How Many Days : ");
    scanf("%f", &Days);
    if (Days < 1){
        printf("Invalid Number\nPlease Enter Again\n");
        printf("Car Rental Services\n");
        printf("How Many Days : ");
        scanf("%f", &Days);
    }
    if (Days <= 3){
        Charge = Days * 1000;
    }
    else if (Days > 3 && Days < 8){
        Charge = Days * 900;
    }
    else if (Days > 7){
        Charge = Days * 850;
    }
    
    if (Charge > 10000){
        Charge *= 0.95;
    }

    printf("Charge : %.2f", Charge);
    return 0;
}