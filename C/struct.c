#include <stdio.h>


typedef struct  {
    int num;
    int size;
}number;


int main() {
    number ID;
    ID.num = 32;
    ID.size = 7;
    printf("num: %d\nsize:%d", ID.num,ID.size);
    return 0;
}