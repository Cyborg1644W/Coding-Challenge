#include <stdio.h>
#include <string.h>

void strCompress(char string[]) {
    int current_index = 0;
    int count = 0;
    char compress[100];
    for(int i = 0; i < strlen(string); i++) {
        if(string[i] == string[i+1]) {
            count++;
        } else {
            count++;
            if(count == 1 || count == 2) {
                compress[current_index] = string[i];
                current_index++;
                if(count == 2) {
                    compress[current_index] = string[i];
                    current_index++;
                }
            } else {
                compress[current_index] = string[i];
                current_index++;
                int digits = sprintf(&compress[current_index], "%d", count);
                current_index += digits;
            }
            count = 0;  
        }
    }
    compress[current_index] = '\0';
    printf("%s\n" , compress);
}

int main() {
    char compress[100] = "AAAAABNANNAAAAEEEEE";
    strCompress(compress);
    return 0;
}
