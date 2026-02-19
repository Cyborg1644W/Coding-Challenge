#include <stdio.h>


int main() {
    int n;
    scanf("%d", &n);
    int arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    if (n == 0) {   
        return 0;
    }

    int longest_start = 0;
    int longest_len = 1;
    int current_start = 0;  
    int current_len = 1;

    for (int i = 1; i < n; i++) {
        if (arr[i] > arr[i - 1]) {
            current_len++;
        } else {
            if (current_len > longest_len) {
                longest_len = current_len;
                longest_start = current_start;
            }
            current_start = i;
            current_len = 1;
        }
    }

    if (current_len > longest_len) {
        longest_len = current_len;
        longest_start = current_start;
    }

    for (int i = 0; i < longest_len; i++) {
        printf("%d ", arr[longest_start + i]);
    }
    printf("\n");

    return 0;
}
