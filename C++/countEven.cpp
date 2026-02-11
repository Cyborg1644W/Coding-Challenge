#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

//function declaration
int getInt(string prompt);
int getPositiveInt(string prompt);
int countEven(int array[], int size);

int main(){
    int array[1000];

    int size = getPositiveInt("Enter Array Length: ");
    
    cout << "\n\nEven Numbers Count :" <<countEven(array, size);

    return 0;
}

int getInt(string prompt) {
    int x;

    while (true) {
        cout << prompt;
        cin >> x;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n'); // simple cleanup
            cout << "Enter a positive integer.\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
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

int countEven(int array[], int size) {
    int evenCounter = 0;
    cout << "Enter " << size << " Numbers:\n";
    for(int i = 0; i < size; i++){
        cout << "Number " << i+1 <<":";
        if((getPositiveInt("") & 1) == 0) {
            evenCounter++;
        }
    }
    return evenCounter;
}