#include <stdio.h>

void TotalMoneyCount(int Amount) {
    int Cash[6] = {100, 50, 20, 10, 5, 1};
    int counter = 0;
    int index = 0;
    while(Amount > 0) {
        if(Amount/Cash[index] >= 1){
            counter += Amount/Cash[index];
            Amount %= Cash[index];
        }else {
            index++;
        }
    }
    printf("%d", counter);
}

int main() {
    TotalMoneyCount(404);
    return 0;
}