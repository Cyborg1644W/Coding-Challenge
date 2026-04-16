#include <iostream>
#include <string>


using std::cin;
using std::endl;
using std::cout;
using std::string;

#ifdef _WIN32 
    void clear_screen() {
        system("cls");
        cout << "\n\n";
    }
#else
    void clear_screen() {
        system("clear");
        cout << "\n\n";
    }    
#endif

class Car {
    string brand;
    string model;
    int year;

public:
    Car(string b, string m, int y) {
        brand = b;
        model = m;
        int year = y;   
    }

    void displayDetails() {
        cout<<"Car Details: " << year << " " << brand << " " << model << endl; 
    }

};

int main() {
    Car myCar("Ford", "Raptor", 2026);

    myCar.displayDetails();
    return 0; 
}