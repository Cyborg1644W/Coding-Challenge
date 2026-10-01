#include <iostream>
#include <fstream>

using namespace std; 

int main() {
    fstream myFile;
    myFile.open("sample.txt", ios::out);
    if (myFile.is_open()) {
        myFile << "I love CS50";
    }
}