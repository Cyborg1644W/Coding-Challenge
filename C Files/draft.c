#include <stdio.h>
#include  <stdlib.h>

int main(){
    int array[100] = {};
    int Num = 0;
    printf("how many Element do you need: ");
    scanf("%d", &Num);

    for(int i = 0; i < Num ; i++){
        printf("Enter Number %d : ",  i+1);
        scanf("%d", &array[i]);
    }


    printf("un ordered : ");
    for (int i = 0; i<Num; i++){
        printf("%d ", array[i]);
    }

    printf("ordered : ");
    for (int i = 0; i<Num-1; i++){
        for(int j = 0; j<Num-1; j++){
            if(array[j] > array[j+1]){
            int temp = array[j];
            array[j] =  array[j+1];
            array[j+1] = temp;
            }
        }  
    }
    for (int i = 0; i<Num; i++){
        printf("%d ", array[i]);
    }
}