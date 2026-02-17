#include <iostream>

using std::cout;
using std::cin;
using std::endl;

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

// void mirror_horizontal(int array[100][100], int size) {
//     for(int i = 0; i < size; i++) {
//         for(int j = size - 1; j >= 0; j--) {
//             cout << array[i][j]<<"\t";
//         }
//         cout << endl;
//     }
// }

int main(){
    int array[100][100] =  {
        {1,2,3},
        {4,5,6},
        {7,8,9}};
    int currentSize = 3;

    display_list(array, currentSize);
    cout << "\n\n";
    cout << "ROTATE TO RIGHT:\n";
    rotate_to_right(array, currentSize);

    return 0;
}