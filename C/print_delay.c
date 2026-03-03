#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define RED "\x1b[31m"
#define RESET "\x1b[0m"


void print_delay(const char *message){
    int len = strlen(message);
    for (int i = 0; i < len; i++){
        printf("%c", message[i]);
        fflush(stdout);
        usleep(30000);
    }
    printf("\n");
}
