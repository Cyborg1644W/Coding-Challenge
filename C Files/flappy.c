#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // For Windows; for Unix, use <termios.h> and implement kbhit/getch equivalents
#include <windows.h>  // For Sleep; for Unix, use usleep

#define HEIGHT 20
#define WIDTH 40
#define PLANE '>'  // Simple plane representation
#define BUILDING '#'  // Building block
#define GAP_SIZE 5  // Gap between top and bottom buildings

int main() {
    int planeY = HEIGHT / 2;  // Plane starts in the middle
    int planeX = 10;  // Fixed X position for the plane
    int score = 0;
    int gameOver = 0;

    // Buildings: each has x position, top height, bottom height
    struct Building {
        int x;
        int topHeight;
        int bottomHeight;
    };
    struct Building buildings[WIDTH];  // Array of buildings

    // Initialize buildings
    for (int i = 0; i < WIDTH; i++) {
        buildings[i].x = WIDTH + i * 10;  // Space them out
        buildings[i].topHeight = rand() % (HEIGHT - GAP_SIZE - 2) + 1;  // Random top height
        buildings[i].bottomHeight = buildings[i].topHeight + GAP_SIZE;  // Bottom starts after gap
    }

    while (!gameOver) {
        // Handle input: space to flap
        if (_kbhit()) {
            char ch = _getch();
            if (ch == ' ') {
                planeY -= 2;  // Flap up
            }
        }

        // Apply gravity
        planeY += 1;

        // Move buildings left
        for (int i = 0; i < WIDTH; i++) {
            buildings[i].x--;
            if (buildings[i].x < 0) {
                buildings[i].x = WIDTH;
                buildings[i].topHeight = rand() % (HEIGHT - GAP_SIZE - 2) + 1;
                buildings[i].bottomHeight = buildings[i].topHeight + GAP_SIZE;
                score++;  // Passed a building
            }
        }

        // Check collision
        if (planeY < 0 || planeY >= HEIGHT) {
            gameOver = 1;
        }
        for (int i = 0; i < WIDTH; i++) {
            if (buildings[i].x == planeX) {
                if (planeY < buildings[i].topHeight || planeY >= buildings[i].bottomHeight) {
                    gameOver = 1;
                }
            }
        }

        // Clear screen (Windows)
        system("cls");

        // Print the game
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == planeX && y == planeY) {
                    printf("%c", PLANE);
                } else {
                    int isBuilding = 0;
                    for (int i = 0; i < WIDTH; i++) {
                        if (buildings[i].x == x) {
                            if (y < buildings[i].topHeight || y >= buildings[i].bottomHeight) {
                                isBuilding = 1;
                                break;
                            }
                        }
                    }
                    printf("%c", isBuilding ? BUILDING : ' ');
                }
            }
            printf("\n");
        }
        printf("Score: %d\n", score);
        printf("Press space to flap. Avoid buildings!\n");

        Sleep(100);  
    }

    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
