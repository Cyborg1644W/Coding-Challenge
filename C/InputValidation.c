#include <stdio.h>
#include <stdlib.h>

char *input(const char *prompt){
    printf("%s", prompt);

    size_t size = 64;
    char *buffer = malloc(size);
    if (!buffer) return NULL;

    size_t len = 0;
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        buffer[len++] = c;

        if (len == size - 1) {
            size *= 2;
            char *newbuf = realloc(buffer, size);
            if (!newbuf) {
                free(buffer);
                return NULL;
            }
            buffer = newbuf;
        }
    }

    buffer[len] = '\0';
    return buffer;
}

int main(void){
    char *user_input = input("Enter your motherfucking name: \n> ");

    printf("Hello mother fucking %s!", user_input);
    free(user_input);

    return 0;
}