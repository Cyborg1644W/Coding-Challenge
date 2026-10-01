#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

struct Player {
    string name;
    int score;
};

void insertionSort(vector<Player>& leaderboard);

int main() {
    vector<Player> leaderboard = {
        {"Charlie", 85},
        {"Alice", 92},
        {"Eve", 85},
        {"Bob", 92},
        {"Diana", 98}
    };

    insertionSort(leaderboard);
    for (const auto &stat : leaderboard) {
        printf("%9s : %d\n", stat.name.c_str(), stat.score);
    }
}

void insertionSort(vector<Player>& leaderboard) {
    for (int i = 1; i < leaderboard.size(); i++) {
        Player key = leaderboard[i];
        int j = i - 1;
        while (j >= 0 && key.name < leaderboard[j].name) {
            leaderboard[j + 1] = leaderboard[j];
            j--;
        }
        leaderboard[j + 1] = key;
    }
}