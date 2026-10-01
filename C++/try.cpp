#include <iostream>

using namespace std;

void displayData(int Array[][4]);


int main(){
    int studentData[3][4] = {{2,3,2,4},{4,4,2,5},{6,8,2,3}};
    displayData(studentData);
    return 0;
}

void displayData(int Array[][4]) {
    for(int i = 0; i < 3; i++){
        for(int j = 0 ; j < 4; j++){
            cout <<Array[i][j];
        }
    }

}