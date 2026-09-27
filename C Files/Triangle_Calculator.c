#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main() {
    int choice; 
    float SemiPerimeter=0, Base=0, Height=0, SideA=0, SideB=0, SideC=0;
    bool IsRunning = true;
    char QuitOption;

    while (IsRunning) {
        printf("\nTriangle Area Calculator\n");
        printf("Choose Triangle Type:\n");
        printf("1 : Right Triangle \n");
        printf("2 : All Sides Given \n");
        printf("3 : Equilateral Triangle\n");
        printf("4 : Quit\n-> ");
        scanf("%d", &choice);

        if(choice == 1){
            printf("Enter Base: ");
            scanf("%f", &SideA);
            printf("Enter Height: ");
            scanf("%f", &SideB);
            printf("Area = %.2f\n", (SideA * SideB)/2);
        }
        else if(choice == 2){
            printf("Enter Side A: ");
            scanf("%f", &SideA);
            printf("Enter Side B: ");
            scanf("%f", &SideB);
            printf("Enter Side C: ");
            scanf("%f", &SideC);

            SemiPerimeter = (SideA + SideB + SideC) / 2;
            printf("Area = %.2f\n", sqrt(SemiPerimeter * (SemiPerimeter - SideA) * (SemiPerimeter - SideB) * (SemiPerimeter - SideC)));
        }
        else if(choice == 3){
            printf("Enter Side Length: ");
            scanf("%f", &SideA);
            printf("Area = %.2f\n", (sqrt(3)/4) * pow(SideA, 2));
        }
        else if(choice == 4){
            IsRunning = false;
            printf("Exiting...\n");
        }
        else{
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
