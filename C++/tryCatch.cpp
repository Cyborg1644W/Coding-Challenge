#include <iostream> // header
using namespace std; 
int main(){
    try {
        // Code that may throw an exception
        cout << "NO error occurred: " << endl;
        }
        catch (int errorCode) {
        cout << "Error occurred: " << errorCode;
        }
}
