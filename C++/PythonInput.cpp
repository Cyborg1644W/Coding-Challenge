#include <iostream>
#include <string>
#include <cctype>

using std::cin;
using std::cout;
using std::string;

// Function Prototypes 
string getStringNoNumbers(string prompt);
int getPositiveInt(string prompt);
int getNegativeInt(string prompt);
double getDouble(string prompt);

// Main 
int main() {
    string name = getStringNoNumbers("Enter your name: ");
    int age = getPositiveInt("Enter your age: ");
    int debt = getNegativeInt("Enter your debt: ");
    double gpa = getDouble("Enter your GPA: ");

    cout << "\nResults:\n";
    cout << "Name: " << name << "\n";
    cout << "Age: " << age << "\n";
    cout << "Debt: " << debt << "\n";
    cout << "GPA: " << gpa << "\n";

    return 0;
}

// Function Definitions 

// String input (letters only)
string getStringNoNumbers(string prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input == "") {
            cout << "Input cannot be empty.\n";
            continue;
        }

        bool valid = true;
        for (char c : input) {
            if (isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (valid)
            return input;

        cout << "Letters only. Try again.\n";
    }
}

// Positive integer
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

// Negative integer
int getNegativeInt(string prompt) {
    int x;

    while (true) {
        cout << prompt;
        cin >> x;

        if (cin.fail() || x >= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Enter a negative integer.\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

// Double input
double getDouble(string prompt) {
    double x;

    while (true) {
        cout << prompt;
        cin >> x;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Enter a number.\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}