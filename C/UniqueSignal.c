#include <stdio.h>
#include <string.h>

void getUnique(char string[]) {
    int length = strlen(string);
    
    for(int i = 0; i < length; i++) {
        int isduplicate = 0; //di duplicate
        for(int j = 0; j < length; j++) {
            if(j == i) {
                continue;
            }
            if(string[i] == string[j]) {
                isduplicate = 1;
                break;
            }
        }
        if(isduplicate == 0) {
            printf("THe first Unique Num is %c", string[i]);
            break;
        }
    }
}

int main() {
    char Characters[100] = "abcdefgabcdegfrabcdefg";
    
    getUnique(Characters);
    return 0;
}