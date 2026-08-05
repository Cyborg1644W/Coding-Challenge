#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int>& arr) {
    for(int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main() {
    vector<int> arr = {5, 2, 8, 1, 9, 3, 6, 7, 8, 3, 8, 3, 6, 2, 1, 6 ,3 , 10, 9, 12};
    
    insertionSort(arr);
    
    cout << "Sorted: ";
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
    
    return 0;
}
