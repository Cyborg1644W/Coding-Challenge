#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

// Pass by value - modifies copy
void movePointByValue(struct Point p, int dx, int dy) {
    p.x += dx;
    p.y += dy;
}

// Pass by pointer - modifies original
void movePointByPointer(struct Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

int main() {
    // Normal struct
    struct Point pt1 = {10, 20};
    
    // Pointer to struct
    struct Point *pt2 = malloc(sizeof(struct Point));
    pt2->x = 5;
    pt2->y = 15;
    
    movePointByValue(pt1, 1, 1);
    printf("After by-value: %d, %d\n", pt1.x, pt1.y); // 10, 20 (unchanged)
    
    movePointByPointer(&pt1, 1, 1);
    printf("After by-pointer: %d, %d\n", pt1.x, pt1.y); // 11, 21 (changed)
    
    movePointByPointer(pt2, 2, 2);
    printf("Pointer struct: %d, %d\n", pt2->x, pt2->y); // 7, 17 (changed)
    
    free(pt2);
    return 0;
}