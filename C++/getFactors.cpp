#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

// function declaration
void getFactors(int& Number, int& counter, int list[]);
int getArraySize(int Array[]);
void sortArray(int Array[]);
void displayArray(int counter, int Array[]);


//main function
int main(){
    int countFactors = 0;
    int Number = 6;
    int factors[100];

    getFactors(Number, countFactors, factors);
    displayArray(countFactors, factors);

    return 0;
}

void getFactors(int& Number, int& counter, int factors[]) {
    counter = 0;
    for(int i = 1; i <= sqrt(Number); i++) {
        if (Number % i == 0){
            factors[counter] = i;
            counter++;
            factors[counter] = Number / i;
            counter++;
        }
    }
}

int getArraySize(int Array[]) {
    int size = sizeof(Array) / sizeof(Array[0]);
    return size;
}

void sortArray(int Array[]) {
    
}

void displayArray(int size, int Array[]) {
    for(int i = 0; i < size ; i++) {
        cout << Array[i] << endl;
    }
}