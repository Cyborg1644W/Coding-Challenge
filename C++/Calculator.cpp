#include <iostream>

using std::cout;
using std::cin;
using std::endl;

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
bool divide(double a, double b, double &result);

int main() {
    
    
}

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

bool divide(double a, double b, double &result) {
    if (b == 0) return false;
    result = a / b;
    return true;
}