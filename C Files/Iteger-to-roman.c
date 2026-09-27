#include <stdio.h>
#include <string.h>

//int to roman
int main(){
    int UserNum;
    // char Roman[50];
    int Values[13] = {1000, 900, 500, 400,
        100, 90, 50, 40,
        10, 9, 5, 4, 1};

    // 13 length , 
    char Symbols[13][3] = {
        "M", "CM", "D", "CD",
        "C", "XC", "L", "XL",
        "X", "IX", "V", "IV", "I"};

    printf("Enrter a Number : ");
    scanf("%d", &UserNum);

    for (int i = 0; i <= 13; i++){
        while (UserNum >= Values[i]){
            printf("%s", Symbols[i]);
            UserNum -= Values[i];
        }
    }

    return 0;
}