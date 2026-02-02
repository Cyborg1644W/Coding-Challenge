#include <iostream>
#include <cstdlib> // Required for system()
#include <unistd.h> // Required for sleep() and usleep()

using std::cout;
using std::cin;
using std::endl;

int main() {
    cout << "Some output before clearing." << endl;
    // The system clear command for macOS/Linux
    
    system("clear"); 
    cout << "\nOutput after clearing." << endl;

    std::cout << "Sleeping for 1 second..." << std::endl;
    sleep(1); // Sleep for 1 second

    std::cout << "Slept for 1 second." << std::endl;
    return 0;
}
