#include <iostream> // header

using namespace std; 

//function declaration
int get_second_lowest(int numbers[], int arraySize);
int get_second_highest(int array[], int arraySize);
int get_mean(int array[], int arraySize);

//main function
int main(){

    //local variables
    int numbers[1000]= {2, 43, 46, 23, 64, 13, 53, 32, 42, 44}; 
    int arraySize = 10;

    //displaying output
    cout << "2nd Lowest : " << get_second_lowest(numbers, arraySize) << endl;
    cout << "2nd Highest : " << get_second_highest(numbers, arraySize) << endl;
    cout << "The Mean is " << get_mean(numbers, arraySize) << endl;
    return 0;
}

int get_second_lowest(int array[], int arraySize){
    int lowest = array[0]<array[1] ? array[0] : array[1]; 
    int secondLowest = array[0]<array[1] ? array[1] : array[0];

    for(int i = 0; i < arraySize; i++){
        if(lowest < array[i]){
            secondLowest = lowest;
            lowest = array[i];
        } else if (secondLowest > array[i]){
            secondLowest = array[i];
        }
    }
    return secondLowest;
}

int get_second_highest(int array[], int arraySize){
    int highest = 0;
    int secondHighest = 0;

    for(int i = 0; i < arraySize; i++){
        if(highest < array[i]){
            secondHighest = highest;
            highest = array[i];
        } else if (secondHighest > array[i]){
            secondHighest = array[i];
        }
    }
    return secondHighest;
}

int get_mean(int array[], int arraySize){
    int sum = 0;

    for(int i = 0; i < arraySize; i++){
        sum += array[i];
    }

    return sum/arraySize;
}


