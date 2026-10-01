#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    char spinner[] = {'|', '/', '-', '\\'}; 
    
    cout << "Loading ";

    for (int i = 0; i < 20; ++i) {
        cout << spinner[i % 4] << "\b"; 
        
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(150));
    }

    cout << "Done! \n";

    return 0;
}