#include <iostream>

namespace first{
    int x = 1;
}

namespace second{
    int x = 2;
}

/*
using std::cout;
using std::string;

is better than

using namespace std;
*/


int main(){
    // using namespace second;
    // std::cout << x;

    // or 
    
    // std::cout << first::x;

    return 0;
}