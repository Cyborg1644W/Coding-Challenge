#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Students {
    string name;
    float score;
};

void insertionsort(vector <Students>& record) {
    for(int i = 1, len = record.size(); i < len; i++) {
        Students key = record[i];
        int j = i - 1;
        while (j >= 0 && key.score < record[j].score) {
            record[j+1] = record[j];
            j--;
        }
        record[j+1] = key;
    }
}

int main() {
    vector <Students> record = {
        {"A", 2.3},
        {"E", 4.2},
        {"C", 1.2}
    };

    for (const auto &stud : record) {
        cout << stud.name << ": " << stud.score << endl;
    }

}

