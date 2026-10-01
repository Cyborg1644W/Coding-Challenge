#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main () {
    fstream File;
    File.open("getinfo.csv", ios::in);

    if(File.is_open()) {
        string name;
        while (getline(File, name)) {
            cout << name << endl;
        }
        File.close();
    }

}