#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void display_list(int array[100][100], int size);
void rotate_to_right(int array[100][100], int size);
void rotate_to_left(int array[100][100], int size);
void mirror_horizontal(int array[100][100], int size);
void mirror_vertical(int array[100][100], int size);

int main(void){
    int array[100][100] =  {
        {1,2,3},
        {4,5,6},
        {7,8,9}};
    int currentSize = 3;

    display_list(array, currentSize);
    cout << "\nROTATE TO RIGHT:\n";
    rotate_to_right(array, currentSize);
    cout << "\nROTATE TO LEFT:\n";
    rotate_to_left(array, currentSize);
    cout << "\nMirror Horizontal:\n";
    mirror_horizontal(array, currentSize);
    cout << "\nMirror Vertical:\n";
    mirror_vertical(array, currentSize);

    return 0;
}

void display_list(int array[100][100], int size) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++){
            cout << array[i][j] << "\t";
        }
        cout << endl;
    }
}

void rotate_to_right(int array[100][100], int size) {
    for(int i = 0; i < size; i++) {
        for(int j = size - 1; j >= 0; j-- ){
            cout << array[j][i] << "\t";
        }
        cout << endl;
    }   
}

void rotate_to_left(int array[100][100], int size) {
    for(int i = size - 1; i >= 0; i--) {
        for(int j = 0; j < size; j++) {
            cout << array[j][i] << "\t";
        }
        cout << endl;
    }
}

void mirror_horizontal(int array[100][100], int size) {
    for(int i = 0; i < size; i++) {
        for(int j = size - 1; j >= 0; j--) {
            cout << array[i][j] << "\t";
        }
        cout << endl;
    }
}

void mirror_vertical(int array[100][100], int size) { 
    for(int i = size - 1; i >= 0; i--) {
        for(int j = 0; j < size; j++) {
            cout << array[i][j] << "\t";
        }
        cout << endl;
    }
}
