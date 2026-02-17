#include <iostream>

int generateRandom(int range){
    int random = (rand() % range) + 1;

    return random;
}

int main(){
    srand(time(NULL));
    std::cout << generateRandom(100);
    return 0;
}