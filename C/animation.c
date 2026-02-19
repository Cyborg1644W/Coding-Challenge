#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define CLEAR "\x1b[2J\x1b[H" // Clear screen

void drawCauldron(int frame) {
    // Different frames for bubbling animation
    char *bubbles[] = {".", "o", "O", "@"};
    char *eye[] = {"-","0","O","O", "O" , "O", "-"};

    printf(CLEAR);
    printf("       %s~ Potion Crafting ~%s\n\n", CYAN, RESET);
    printf("      %s    (    )    %s\n", YELLOW, RESET);
    printf("      %s     )  (     %s\n", YELLOW, RESET);
    printf("      %s    (    )    %s\n", YELLOW, RESET);
    printf("      %s   .-====-.   %s\n", MAGENTA, RESET);
    printf("      %s  /        \\  %s\n", MAGENTA, RESET);
    printf("      %s |          | %s\n", MAGENTA, RESET);
    printf("      %s | %s %s   %s%s   | %s\n", MAGENTA, RED, eye[frame%7],eye[frame%7], MAGENTA, RESET);
    printf("      %s |          | %s\n", MAGENTA, RESET);
    printf("      %s  \\__/  \\__/ %s\n\n", MAGENTA, RESET);

    printf("%sIngredients:%s\n", YELLOW, RESET);
    printf("  %s[1]%s Crimson Flower\n", RED, RESET);
    printf("  %s[2]%s Moon Dew\n", BLUE, RESET);
    printf("  %s[3]%s Mystic Herb\n", GREEN, RESET);
    printf("\nMixing potion... Press Ctrl+C to exit animation.\n");
}

int main() {
    int frame = 0;  

    while(1) {
        drawCauldron(frame);
        usleep(300000); 
        frame++;
    }

    return 0;
}