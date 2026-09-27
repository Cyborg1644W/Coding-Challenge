#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n = 5;
    vector<bool> dp(n + 1, false);

    for (bool c : dp) {
        cout << c << ", ";
    }
}
