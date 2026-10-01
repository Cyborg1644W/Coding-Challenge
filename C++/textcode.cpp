#include <iostream>

using namespace std;

int factorial(int x, char symbol) {
    for(int i = 0; i < x; i++) {
        cout << symbol;
    }
    cout << endl;

    if (x <= 1) {
        return 1;
    }
    return x * factorial(x-1, symbol);
}

int main() {
    char letter = '#';
    int num = 5;
    factorial(num, letter);
}