#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int number;
    struct node *next;
} node;

int main(void) {
    node *list = NULL;
    
    for (int i = 0, size = 4; i < size; i++) {
        node *n = malloc(sizeof(node));
        if (n == NULL) {
            return 0;
        }

        n->number = i;
        n->next = list;
        list = n;
    }

    for (node *ptr = list; ptr != NULL; ptr = ptr->next) {
        printf("%d\n", ptr->number);
    }
    return 0;
}