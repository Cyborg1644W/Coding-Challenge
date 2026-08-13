#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int number;
    struct node *next;
} node;

int main(void) {
    int size = 5;

    node *list = NULL;

    for (int i = 0; i < size; i++) {
        node *n = malloc(sizeof(node));
        if (n == NULL) {
            return 1;
        }

        n->number = i;
        n->next = list;
        list = n;
    }

    // Print the list
    node *cursor = list;
    while (cursor != NULL) {
        printf("%i\n", cursor->number);
        cursor = cursor->next;
    }

    // Free the list (don't forget this!)
    cursor = list;
    while (cursor != NULL) {
        node *next = cursor->next;
        free(cursor);
        cursor = next;
    }

    return 0;
}
