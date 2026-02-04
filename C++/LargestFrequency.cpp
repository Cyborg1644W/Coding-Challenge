#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

int getPositiveInt(string prompt);
void getArray(int array[], int& arraySize);
int getLargest(int array[], int arraySize);
void frequency(int array[], int arraySize, int largest);

int main(){
    int numbers[100];
    int numSize = 0;

    getArray(numbers, numSize);
    int largest = getLargest(numbers, numSize);
    frequency(numbers, numSize, largest);
    return 0;
}

int getPositiveInt(string prompt) {
    int x;

    while (true) {
        cout << prompt;
        cin >> x;

        if (cin.fail() || x <= 0) {
            cin.clear();
            cin.ignore(1000, '\n'); // simple cleanup
            cout << "Enter a positive integer.\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

void getArray(int array[], int& arraySize) {
    int num = getPositiveInt("How Many Number Would You Like to Enter : ");
    for(int i = 0; i < num; i++){
        cout << "Enter Number " << i+1;
        array[i] = getPositiveInt(": ");
        arraySize++;
    }

}

int getLargest(int array[], int arraySize) {
    int largest = array[0]; 
    for (int i = 1; i < arraySize; i++){
        if(array[i] > largest){
            largest = array[i];
        }
    }
    cout << "The Largest Number is " << largest << endl;
    return largest;
}

void frequency(int array[], int arraySize, int largest) {
    int count = 0;
    for (int i = 0; i < arraySize; i++){
        if(array[i] == largest){
            count++;
        }
    }
    cout << "Frequency : " << count << endl;
}