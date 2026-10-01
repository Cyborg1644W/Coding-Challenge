#include <iostream>
#include <cstdlib> // Required for system()
#include <string>
#include <cctype>
#include <iomanip> // For table formatting

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::left;
using std::setw;

int numbers[5] = {};

int main(){

    for(int i = 0; i < 5; i++){
        cout << "Input 5 numbers: "; 
        cin >> numbers[i]; 
    }  

    cout << "these are the 5 numbers: ";

    for(int i = 0; i < 5; i++){
        cout << numbers[i] << "\n";
    }
}