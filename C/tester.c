#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32 //For Windows
    #include <conio.h>
    #include <windows.h>
    #include <conio.h>
    #define getch() _getch()
    #define SLEEP_MS(ms) Sleep(ms)
    #define CLEAR_SCREEN() system("cls")

#else //for MacOS
    #include <termios.h> 
    #include <unistd.h>
    #include <sys/select.h>
    #define SLEEP_MS(ms) usleep(ms * 1000)
    #define SLEEP_US(us) usleep(us)
    #define CLEAR_SCREEN() system("clear")
    #define getch() getch_posix()
#endif

#define BOLD "\033[1m"
#define RED "\033[38;2;255;0;70m" 
#define ORANGE "\033[38;2;255;140;0m" 
#define YELLOW "\033[38;2;255;255;0m" 
#define NEON_GOLD "\033[38;2;255;215;0m" 
#define NEON_LIME "\033[38;2;0;255;128m" 
#define GREEN "\033[38;2;57;255;20m" 
#define CYAN "\033[38;2;0;255;255m"
#define BLUE "\033[38;2;0;150;255m" 
#define NEON_AQUA "\033[38;2;0;255;200m" 
#define NEON_SKY "\033[38;2;0;180;255m"
#define NEON_PURPLE "\033[38;2;200;0;255m"
#define NEON_VIOLET "\033[38;2;180;0;255m"
#define MAGENTA "\033[38;2;255;0;200m" 
#define NEON_PINK "\033[38;2;255;20;147m"
#define NEON_WHITE "\033[38;2;245;245;255m"
#define GRAY "\x1b[90m"
#define RESET "\033[0m"
#define DELETEPREVIOUS "\033[F\033[K"
#define LOADING_BAR_LENGTH 75

// █ ░
void render_loading_bar(char *color){
    for(int i = 0; i<=LOADING_BAR_LENGTH; i++){
        int filled = i;
        int notFilled = LOADING_BAR_LENGTH - i;
        printf("\r[");
        for(int fil = 1; fil<=filled; fil++){
            printf("█");
        }
        for(int not_fil = 1; not_fil<=notFilled; not_fil++){
            printf("░");
        }
        printf("]");
        SLEEP_MS(20);
        fflush(stdout);
    }
}


int main(){ 
    render_loading_bar(BLUE);
    return 0;
}