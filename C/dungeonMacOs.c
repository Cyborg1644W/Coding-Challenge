#include <stdio.h>
#define GREEN "\x1b[32m"

void render_bar(int current_hp, int max_hp) {
    const int BAR_LENGTH = 50;
    float health_ratio = (float)current_hp / max_hp;
    int filled_chars = (int)(health_ratio * BAR_LENGTH);
    int empty_chars = BAR_LENGTH - filled_chars;
    printf("[");
    for (int i = 0; i < filled_chars; i++) {
        printf("█"); 
    }
    for (int i = 0; i < empty_chars; i++) {
        printf("░");
    }
    printf("] %d/%d\n", current_hp, max_hp);
}

int main() {
    render_bar(500, 500); 
    render_bar(300, 500);  
    render_bar(100, 500);  
    return 0;
}