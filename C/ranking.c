#include <stdio.h>

#define MAX_DRIVERS 10
#define MAX_CHAR 300

typedef struct {
    int time;
    char names[100];
}Driver;

int main() {
    Driver leaderboard[] = {
        {38, "Reindel"},
        {29, "Ivan"},
        {42, "Alex"}
    };

    printf("Players:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d.) %s - %d minutes\n", i + 1, leaderboard[i].names, leaderboard[i].time);
    }
    for (int i = 0; i < 3 - 1; i++){
        for (int j = 0; j < 3 - i - j; j++){
            if (leaderboard[j].time > leaderboard[j+1].time) {
                Driver tmp = leaderboard[j+1];
                leaderboard[j + 1] = leaderboard[j];
                leaderboard[j] = tmp;
            }
        }
    }
    printf("Ranks:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d.) %s - %d minutes\n", i + 1, leaderboard[i].names, leaderboard[i].time);
    }
}

