#include <stdio.h>

void countHailStoneSequence(int number) {
    int counter = 0;   
    while(number > 1){
        if((number & 1) == 1) {
            number *= 3;
            number++;
        } else{
            number /= 2;
        }
        counter++;
    }
    printf("%d", counter);
}

int main() {
    countHailStoneSequence(6);
    return 0;
}