//OS: Window
//IDE: VS Code

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

double calculateBonus(double sales) {
    if (sales <= 3000)
        return 0.04 * sales;
    else if (sales <= 10000)
        return 120 + 0.07 * (sales - 3000);
    else
        return 620 + 0.10 * (sales - 10000);
}

double longestDistance() {
    double score = 0, maxScore = 0;

    printf("You will enter distances run by participants.\n");
    printf("Type 0 or a negative number when you are done.\n");

    while (true) {
        printf("Enter distance (number only, e.g., 5.5): ");
        if (scanf("%lf", &score) != 1) {
            printf("Invalid input. Please type a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (score <= 0) break;
        if (score > maxScore) maxScore = score;
    }

    return maxScore;
}

void payrollLogic() {
    char name[50];
    double hours, rate, gross, overtime = 0;

    printf("Enter employee name (letters only, no spaces): ");
    scanf(" %s", name);

    printf("Enter hours worked (number only, e.g., 40): ");
    if (scanf("%lf", &hours) != 1 || hours < 0) {
        printf("Invalid input. Hours cannot be negative.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter hourly rate (number only, e.g., 200): ");
    if (scanf("%lf", &rate) != 1 || rate < 0) {
        printf("Invalid input. Rate cannot be negative.\n");
        while (getchar() != '\n');
        return;
    }

    if (hours > 40) {
        overtime = hours - 40;
        gross = 40 * rate + overtime * rate * 1.5;
    } else {
        gross = hours * rate;
    }

    printf("\n-------- PAYROLL SUMMARY --------\n");
    printf("Employee Name: %s\n", name);
    printf("Total Hours Worked: %.2lf hours\n", hours);
    printf("Gross Pay: Php %.2lf\n", gross);
    if (overtime > 0)
        printf("Overtime Pay: Php %.2lf\n", overtime * rate * 1.5);
    printf("---------------------------------\n");
}

double largestTransaction() {
    double transaction = 0, largest = 0;

    printf("Enter transaction amounts one by one.\n");
    printf("Type '0' or a 'negative number' to finish.\n");

    while (true) {
        printf("Enter transaction amount (number only, e.g., 1500): ");
        if (scanf("%lf", &transaction) != 1) {
            printf("Invalid input. Please type a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (transaction <= 0) break;
        if (transaction > largest) largest = transaction;
    }

    return largest;
}

int main() {
    int choice;
    bool isRunning = true;

    do {
        system("cls");
        printf("\n============= MAIN MENU =============\n");
        printf("1 : Bonus Calculation for Sales Staff\n");
        printf("2 : Record Longest Distance Run\n");
        printf("3 : Simple Payroll Calculation\n");
        printf("4 : Find Largest Business Transaction\n");
        printf("5 : Exit Program\n");
        printf("=====================================\n");
        printf("Enter your choice (1-5, Number Only): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Type a number from 1 to 5.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("\n");

        switch (choice) {
            case 1: {
                system("cls");
                double sales;
                printf("Enter total sales for the month (number only, e.g., 5000): ");
                if (scanf("%lf", &sales) != 1 || sales < 0) {
                    printf("Invalid input. Sales must be 0 or higher.\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Calculating bonus...\n");
                Sleep(1000);
                printf("Your bonus is: Php %.2lf\n", calculateBonus(sales));
                break;
            }

            case 2: {
                system("cls");
                double maxScore = longestDistance();
                printf("Calculating result...\n");
                Sleep(1000);
                printf("The longest distance recorded is: %.2lf\n", maxScore);
                break;
            }

            case 3: { 
                system("cls");
                printf("Enter employee details carefully as requested.\n");
                payrollLogic();
                break;
            }

            case 4: { 
                system("cls");
                double largest = largestTransaction();
                printf("Calculating result...\n");
                Sleep(1000);
                printf("The largest transaction recorded is: Php %.2lf\n", largest);
                break;
            }

            case 5: 
                system("cls");
                printf("Exiting program. Goodbye!\n");
                Sleep(1000);
                system("cls");
                isRunning = false;
                break;

            default:
                printf("Invalid choice. Please select a number from 1 to 5.\n");
                break;
        }

        if (isRunning) {
            printf("\nPress Enter to return to the main menu...");
            getchar(); getchar();
        }

    } while (isRunning);

    return 0;
}
