//IDE : VS Code

#include <stdio.h>
#include <stdlib.h>

int main(){
    char name[20];
    float TotalSales=0.0f, Bonus=0.0f, PagIbig=0.0f, SSS=0.0f, WISP=0.0f, WithholdingTax=0.0f;
    const int Salary=15000;

    printf("Enter Your Name : ");
    scanf("%s", name);
    printf("Enter Total Sales : ");
    scanf("%f", &TotalSales);
    printf("Enter Bonus : ");
    scanf("%f", &Bonus);

    float Commission = TotalSales * 0.125;
    float GrossPay = Salary + Commission + Bonus;


    // Pag Ibig
    if (GrossPay <= 1500){
        PagIbig = GrossPay * 0.01;
    }
    else if (GrossPay > 1500){
        PagIbig = GrossPay * 0.02;
    }
    
    if (PagIbig > 100){
        PagIbig = 100;
    }


    // SSS
    if (GrossPay <= 10000){
        SSS = 500;
    }
    else if (GrossPay <= 20000){
        SSS = 1000;
    }
    else if (GrossPay > 20000){
        SSS = 1500;
    }
    

    // WISP
    if (GrossPay <= 20000){
        WISP = 300;
    }
    else if (GrossPay > 20000){
        WISP = 500;
    }
    

    // Withholding Tax
    if (GrossPay <= 20833){
        WithholdingTax = 0;
    }
    else if (GrossPay > 20833 && GrossPay <= 33333){
        WithholdingTax = 0.2 * (GrossPay - 20833);
    }
    else if (GrossPay > 33333 && GrossPay <= 66667){
        WithholdingTax = 2500 + (0.25 * (GrossPay - 33333));
    }
    else if (GrossPay > 66667){
        WithholdingTax = 10833 + (0.3 * (GrossPay - 66667));
    }

    //Total Deduction
    float TotalDeduction = PagIbig + SSS + WISP + WithholdingTax;
    float NetPay = GrossPay - TotalDeduction;


    //display info
    printf("%-16s : %-5s\n","Account Name", name);
    printf("%-16s : %-5d\n","Salary", Salary);
    printf("%-16s : %-5.2f\n","Commission", Commission);
    printf("%-16s : %-5.2f\n","Bonus", Bonus);
    printf("%-16s : %.2f\n","GrossPay", GrossPay);
    printf("\n========DEDUCTION========\n");
    printf("%-16s : %.2f\n","Pag Ibig", PagIbig);
    printf("%-16s : %.2f\n","SSS", SSS);
    printf("%-16s : %.2f\n","WISP", WISP);
    printf("%-16s : %.2f\n","Tax", WithholdingTax);
    printf("%-16s : %.2f\n","Total", TotalDeduction);
    printf("%-16s : %.2f","Net Pay", NetPay);

    return 0;
}