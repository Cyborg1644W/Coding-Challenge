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

//colors
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

#define CENTER "\t\t\t\t\t\t"
#define INDENT "\t\t\t\t"
#define USERS 5
#define NAME_LEN 20
#define LOADING_BAR_LENGTH 50
#define LOADING_SPEED 30 // ms

void render_loading_bar(char *color);
void wait_for_enter();
int getPositiveNumber(char *color);
void print_header(const char *word,char *color);
void print_body(const char *word, char *color);
void show_menu();
int get_menu_choice();
void get_balance(int *current_user);
void deposit_money(int *current_user);
void withdraw_money(int *current_user);
void transfer_money();
void logout();
int login(int *current_user);

int isRunning = true;
int accountNumbers[USERS] = {1001, 1002, 1003, 1004, 1005};
int pins[USERS] = {2131, 1433, 6732, 8012, 1273};
char names[USERS][NAME_LEN] = {"Reindel", "Bob", "Charlie", "David", "Eve"};
int balances[USERS] = {1000, 500, 750, 1200, 300};

int main(){
    printf("\033[?25l");
    CLEAR_SCREEN();

    int choice = 0;
    int current_user = -1;
    
    while (1){
        if (login(&current_user) == 0 ) break;
    }
    render_loading_bar(YELLOW);
    while(isRunning){
        choice = get_menu_choice();
        switch(choice){ 
            case 1:
                get_balance(&current_user);
                break;
            case 2:
                deposit_money(&current_user);
                break;
            case 3:
                withdraw_money(&current_user);
                break;
            case 4:
                transfer_money();
                break;
            case 5:
                logout(); 
                break;
        }
    }
    return 0;
}

void render_loading_bar(char *color){
    CLEAR_SCREEN();
    printf("\n\n\n");
    for(int i = 0; i<=LOADING_BAR_LENGTH; i++){
        int filled = i;
        int notFilled = LOADING_BAR_LENGTH - i;
        printf("\r"INDENT"[%s", color);
        for(int fil = 1; fil<=filled; fil++){
            printf("█");
        }
        printf(RESET);
        for(int not_fil = 1; not_fil<=notFilled; not_fil++){
            printf("░");
        }
        printf("%s]",RESET);
        SLEEP_MS(LOADING_SPEED);
        fflush(stdout);
    }
}

void wait_for_enter(){
    printf("\n");
    print_body(">> Press Enter to Continue <<",YELLOW);
    getchar();
}

int getPositiveNumber(char *color){
    int num;
    char c;
    printf("%s",color);
    while (scanf("%d%c", &num, &c) != 2 || c != '\n' || num <= 0) {
        printf(RESET);
        CLEAR_SCREEN();
        print_header("Invalid input", RED);
        wait_for_enter();
        printf(DELETEPREVIOUS DELETEPREVIOUS DELETEPREVIOUS);
        print_body("Enter a Number: " ,color);
    }
    printf(RESET);
    return num;
}

void print_header(const char *word,char *color){
    int count = 0;
    const char *p = word;
    while (*p != '\0') {
        count++;
        p++;
    }
    printf("\n\t\t\t\t============================================\n\t\t\t\t");//44
    printf("%s",color);
    for(int i = 0; i <= (43 - count)/2; i++){
        printf(" ");
    }
    printf("%s\n",word);
    printf(RESET);
    printf("\t\t\t\t============================================\n");
}

void print_body(const char *word, char *color){
    int count = 0;
    const char *p = word;
    while (*p != '\0') {
        count++;
        p++;
    }
    printf("\t\t\t\t");
    printf("%s",color);
    for(int i = 0; i <= (43 - count)/2; i++){
        printf(" ");
    }
    printf("%s",word);
    printf(RESET);
}

void show_menu(){
    CLEAR_SCREEN();
    print_header("ATM MACHINE",GREEN);
    printf(CENTER NEON_PURPLE"[1]" RESET" Balance\n");
    printf(CENTER NEON_PURPLE"[2]" RESET" Deposit\n");
    printf(CENTER NEON_PURPLE"[3]" RESET" Withdraw\n");
    printf(CENTER NEON_PURPLE"[4]" RESET" Transfer\n");
    printf(CENTER NEON_PURPLE"[5]" RESET" Logout\n");    
}

int get_menu_choice(){
    show_menu();
    int userChoice = 0;
    do {
        printf("\n");
        print_body("Enter a positive number: ",YELLOW);
        userChoice = getPositiveNumber(BLUE);
        CLEAR_SCREEN();
        show_menu();
        printf("\n");
        print_body("1 - 5 Only", RED);
        wait_for_enter();
        printf(DELETEPREVIOUS DELETEPREVIOUS DELETEPREVIOUS);
        print_body("Enter a positive number only: ", YELLOW);
        userChoice = getPositiveNumber(BLUE);  
    } while (userChoice > 5);
    return userChoice;
}

void get_balance(int *current_user){
    CLEAR_SCREEN();
    print_header("BALANCE",YELLOW);
    printf("\n");
    printf(CENTER"SAVINGS : %d", balances[*current_user]);
    printf("\n");
    wait_for_enter();
}

void deposit_money(int *current_user){
    int amount = 0;
    CLEAR_SCREEN();
    print_header("DEPOSIT", YELLOW);
    print_body("Enter Amount: ",BLUE);
    amount = getPositiveNumber(CYAN);
    if(amount <= 0){
        CLEAR_SCREEN();
        print_header("Amount Invalid",RED);
        wait_for_enter();
        return;
    }else{
        balances[*current_user] += amount;
        print_header("Transaction Complete",RED);
        wait_for_enter();
    }
}
 
void withdraw_money(int *current_user){
    CLEAR_SCREEN();
    print_header("WITHDRAW", YELLOW);
    print_body("Enter Amount: ",BLUE);
    int amount = getPositiveNumber(GREEN);
    if(amount % 100 && amount > 0){
        print_body("Please enter an amount in multiples of ₱100.",RED);
    }else if(amount > balances[*current_user]){
        print_body("Insuficient Funds", RED);
    }else{
        balances[*current_user] -= amount;
    }
}

void transfer_money(){
    CLEAR_SCREEN();
    wait_for_enter();
}

void logout(){
    CLEAR_SCREEN();
    isRunning = false;
    printf("BYE");
    wait_for_enter();
} 

int login(int *current_user){
    int accNumber;
    int accPin;
    print_header("ATM MACHINE",YELLOW);
    print_body("Enter Account Number : ",BLUE);
    accNumber = getPositiveNumber(NEON_AQUA);
    print_body("Enter Pin (4digit number) : ", BLUE);
    accPin = getPositiveNumber(NEON_AQUA);

    for (int i = 0; i < USERS; i++) {
        if (accountNumbers[i] == accNumber && pins[i] == accPin) {
            *current_user = i;
            return 0; // Success
        }else if(accountNumbers[i] == accNumber && pins[i] != accPin){
            CLEAR_SCREEN();
            printf("\n");
            print_header("PIN incorrect. Re-enter PIN.", RED);
        }else{
            CLEAR_SCREEN();
            printf("\n");
            print_header("Account number not recognized.", RED);
        }
    }
    wait_for_enter();
    CLEAR_SCREEN();
    return 1; // Failure
}