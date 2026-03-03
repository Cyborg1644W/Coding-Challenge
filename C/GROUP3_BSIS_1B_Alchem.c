#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

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

    static int getch_posix(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #define getch() getch_posix()
#endif

//text
#define BOLD "\033[1m"
#define RED "\033[38;2;255;0;70m" // DAMAGE
#define ORANGE "\033[38;2;255;140;0m" // EXP.
#define YELLOW "\033[38;2;255;255;0m" // GOLD.
#define NEON_GOLD "\033[38;2;255;215;0m" // VITALITY
#define NEON_LIME "\033[38;2;0;255;128m" // POTIONS
#define GREEN "\033[38;2;57;255;20m" // HP
#define CYAN "\033[38;2;0;255;255m"
#define BLUE "\033[38;2;0;150;255m" // STATS
#define NEON_AQUA "\033[38;2;0;255;200m" // species
#define NEON_SKY "\033[38;2;0;180;255m"
#define NEON_PURPLE "\033[38;2;200;0;255m"
#define NEON_VIOLET "\033[38;2;180;0;255m"
#define MAGENTA "\033[38;2;255;0;200m" // ITEMS
#define NEON_PINK "\033[38;2;255;20;147m"
#define NEON_WHITE "\033[38;2;245;245;255m"
#define GRAY "\x1b[90m"
#define RESET "\033[0m"

#define FILE_NAME "paradoxical.dat"
#define EXP_THRESHOLD_BASE 100
#define DATE_SIZE 30
#define NAME_SIZE 50
#define GOLD_STREAK 1.2 //20 %
#define EXP_STEAK 1.5
#define POTION_PRICE 50
#define ELIXIR_PRICE 100
#define VITALITY_VIAL_PRICE 150
#define POTION_SELL_PRICE 30
#define ELIXIR_SELL_PRICE 80
#define POTION_HEAL 50
#define LODGE_REST_COST 30
#define VIAL_HP_BOOST 10
#define BASE_DUNGEON_DAMAGE 10
#define LOADING_TIME_SECONDS 3
#define BAR_LENGTH 50. //loading bar
#define TOTAL_ITERATIONS 100
#define TYPING_SPEED_MS 70 //typing effect

typedef enum {
    ELF = 0,    // +10% Coin
    HUMAN = 1,  // +10% EXP Gain
    GIANT = 2   // +10% Damage Reduction
} Species;

typedef struct {
    char name[NAME_SIZE];
    Species species;
    int hp;
    int maxHp;
    int gold;
    int exp;
    int level;
    int potions;
    int elixirs;
    int vitality_vials;
    int saved_state;
    int revival_used;
    int in_dungeon_seal; //1 = item seal
    int potion_used_in_stage; // 0 not used yet
    int streak;
} Player;

typedef struct {
    const char *title;
    const char *color;
    const char *description;
    const char *subtitle;
} LoreEntry;

//Utility
void clearScreen(void);
static void flush_input(void);
static void wait_for_enter(void);
int menu_select(const char *title, const char *options[], int count, Player *player);
int menu_select_simple(const char *title, const char *options[], int count);
void displayStats(Player *player);
void initPlayer(Player *player);
const char* getSpeciesName(Species s);
void print_delay(const char *message);
void display_title_screen(int frame); // Game title
void display_crafting_title_screen(int frame); // Crafting title
void display_loading_bar(void);
void run_loading_sequence(void);
void show_needle_crafting_animation(void);
void show_paradoxical(void);

// Library Functions
void library(Player *player);

//Input Validation
int getValidInt(char *prompt);
float getValidFloat(char *prompt);
static int get_char_no_wait();

//game Logics
void saveGame(Player *player);
int loadGame(Player *player, char *date_buffer);
void promptTutorial(Player *player);
void runTutorial(Player *player);
void townHub(Player *player);
void generalStore(Player *player);
void lodge(Player *player);
void dungeon(Player *player);
int levelUp(Player *player);
int useItem(Player *player);
void applySpeciesBonus(Player *player, int *expGain, int *goldGain);
void playSnakeGame(Player *player);
void guessTheNumberGame(Player *player);
void amusementPark(Player *player);

// Puzzle Prototypes
int puzzleLevel1(void);
int puzzleLevel2(void);
int puzzleLevel3(void);
int puzzleLevel4(void);
int puzzleLevel5(void);

// Animation Prototypes
void drawCauldron(int frame);
char getBubble(int state);

#ifdef _WIN32
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

int main() {
    Player player;
    srand((unsigned int)time(NULL));
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);//sa bar
#endif
    char saved_date[DATE_SIZE] = "N/A";

    //STATSSSSS
    player.hp = 100;
    player.maxHp = 100;
    player.gold = 50;
    player.exp = 0;
    player.level = 1; 
    player.potions = 0;
    player.elixirs = 0;
    player.vitality_vials = 0;
    player.saved_state = 0;
    player.streak = 0;
    player.species = HUMAN;
    strcpy(player.name, "Novice");

    clearScreen();
    printf(BLUE  "\n[===========================================================================================================================]\n\n" RESET);
    printf("%s", GREEN);
    printf(" \t\t  ######    #####   ######    #####   ######    #####   ##   ##  ##   ######   #####   ##    \n");
    printf(" \t\t  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##     \n");
    printf(" \t\t  ######   #######  #######  #######  ##   ##  ##   ##    ###    ##  ##       #######  ##      \n");
    printf(" \t\t  ##       ##   ##  ##  ##   ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##      \n");
    printf(" \t\t  ##       ##   ##  ##   ##  ##   ##  ######    #####   ##   ##  ##   ######  ##   ##  ###### %s\n", RESET);
    printf(BLUE"\n[===========================================================================================================================]\n\n" RESET);

    //load game mechanics
    int start_new_game = 0;
    if (loadGame(&player, saved_date)) {
        printf(GREEN " Save file found: Main Slot\n" RESET);
        printf(CYAN " Last Saved: %s\n" RESET, saved_date);
        printf(YELLOW " Loaded Stats:"NEON_PINK" %s"NEON_AQUA" the %s |"BLUE" Lvl %d |"GREEN" HP %d/%d |"YELLOW" Gold %d |\n" RESET,
            player.name, getSpeciesName(player.species), player.level, player.hp, player.maxHp, player.gold);
        const char *load_menu[] = {" Continue the Saved Game?", " Start a New Game? " RED "(WARNING: Overwrites Save!)" RESET };
        flush_input();
        int load_choice = 0;
        //load ggame promp
while(1) {
            printf("\n\n\n\n\t\t\t\t  "CYAN"    [===============[ %s "CYAN"]===============]"RESET"\n\n", ORANGE "LOAD GAME" RESET);
            for (int i = 0; i < 2; ++i) {
                if (i == load_choice) {
                    printf(GREEN "\t\t\t\t       >> %s\n" RESET, load_menu[i]);
                } else {
                    printf("\t\t\t\t\t  %s\n", load_menu[i]);
                }
            }
            int key = getch();
#ifdef _WIN32
            if (key == 0 || key == 224) {
                int special = getch();
                if (special == 72) { load_choice = (load_choice - 1 + 2) % 2; }
                else if (special == 80) { load_choice = (load_choice + 1) % 2; }
            } else if (key == 13) { break; }
#else
            if (key == 27) {
                int k1 = getch();
                if (k1 == 91) {
                    int k2 = getch();
                    if (k2 == 65) { load_choice = (load_choice - 1 + 2) % 2; }
                    else if (k2 == 66) { load_choice = (load_choice + 1) % 2; }
                }
            } else if (key == '\n' || key == '\r') { break; }
#endif
            clearScreen();
   		    printf(BLUE  "\n[===========================================================================================================================]\n\n" RESET);
  		    printf("%s", GREEN);
            printf(" \t\t  ######    #####   ######    #####   ######    #####   ##   ##  ##   ######   #####   ##    \n");
            printf(" \t\t  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##     \n");
            printf(" \t\t  ######   #######  #######  #######  ##   ##  ##   ##    ###    ##  ##       #######  ##      \n");
            printf(" \t\t  ##       ##   ##  ##  ##   ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##      \n");
            printf(" \t\t  ##       ##   ##  ##   ##  ##   ##  ######    #####   ##   ##  ##   ######  ##   ##  ###### %s\n", RESET);
            printf(BLUE"\n[===========================================================================================================================]\n\n" RESET);
            printf(GREEN " Save file found: Main Slot" RESET);
            printf(CYAN "\n Last Saved: %s" RESET, saved_date);
            printf(YELLOW "\n Loaded Stats:"NEON_PINK" %s"NEON_AQUA" the %s |"BLUE" Lvl %d |"GREEN" HP %d/%d |"YELLOW" Gold %d\n" RESET,
            player.name, getSpeciesName(player.species), player.level, player.hp, player.maxHp, player.gold);
        }
        if (load_choice == 0) {
            const char *message1 = "\n\n\n\n\n\n\n\n\n Save loaded successfully. Welcome back!";
            printf(GREEN);
            print_delay(message1);
            printf(RESET);
            printf("\033[A\033[K");
            player.saved_state = 0;
        } else {
            start_new_game = 1;
        }
    } else {
        start_new_game = 1;
    }

    if (start_new_game) {
        run_loading_sequence();
        initPlayer(&player);
        promptTutorial(&player);
    }
    const char *main_menu[] = {"Visit Town Hub", "Enter Dungeon", "Inventory", "Save Game", "Exit"};
    while (1) {
        int sel = menu_select(CYAN "MAIN MENU" RESET, main_menu, 5, &player);
        // Remap menu choices to switch cases
        // 0: Town, 1: Dungeon, 2: Inventory, 3: Save, 4: Exit
        if (sel == 3) { // Save Game
            sel = 3;
        } else if (sel == 4) { // Exit
            sel = 4;
        }
                switch (sel) {
                    case 0:
                        townHub(&player);
                        break;
                    case 1:
                        if (player.hp > 0) {
                            const char *dungeon_confirm[] = {"Yes, enter the dungeon", "No, return to safety"};
                            flush_input();
                            int confirm_choice = menu_select(RED "CONFIRM ENTRY" RESET, dungeon_confirm, 2, &player);
                            if (confirm_choice == 0) {
                                dungeon(&player);
                            }
                        } else {
                            const char *message2 = "\n You are too weak! Visit the Lodge or use an item first.\n";
                            printf(RED);
                            print_delay(message2);
                            printf(RESET);
                            wait_for_enter();
                            printf("\033[A\033[K");
                        }
                        break;
                    case 2:
                        useItem(&player);
                        break;
                    case 3:
                        {
                            const char *confirm_menu[] = {"Yes, overwrite save file", "No, cancel save"};
                            flush_input();
                            int confirm = menu_select(YELLOW "CONFIRM SAVE" RESET, confirm_menu, 2, &player);
                            if (confirm == 0) {
                                saveGame(&player);
                                printf(GREEN);
                                const char *message3 = " Game saved successfully!";
                                print_delay(message3);
                                printf(RESET);
                                printf("\033[A\033[K");
                            } else {
                                printf(RED);
                                const char *message4 = " Save operation cancelled.";
                                print_delay(message4);
                                printf(RESET);
                                printf("\033[A\033[K");
                            }
                        }
                        break;
                    case 4:
                        {
                            const char *exit_confirm[] = {"Yes, exit the game", "No, return to main menu"};
                            flush_input();
                            int exit_choice = menu_select(RED " CONFIRM EXIT" RESET, exit_confirm, 2, &player);
                            if (exit_choice == 0) {
                                if (player.saved_state == 1) {
                                    printf(YELLOW "\nWARNING: You have unsaved progress!\n" RESET);
                                    const char *save_prompt[] = {"Yes, save before exiting", "No, exit without saving (Lose progress)", "Cancel exit"};
                                    flush_input();
                                    int save_choice = menu_select(RED " SAVE BEFORE EXIT?" RESET, save_prompt, 3, &player);
                                    if (save_choice == 0) {
                                        saveGame(&player);
                                        printf(GREEN);
                                        const char *message5 = "\n Game saved.";
                                        print_delay(message5);
                                        printf(RESET);
                                        printf("\033[A\033[K");
                                    } else if (save_choice == 2) {
                                        printf(YELLOW);
                                        const char *message6 = "\n Exit cancelled.";
                                        print_delay(message6);
                                        printf(RESET);
                                        printf("\033[A\033[K");
                                        continue;
                                    }
                                }
                                printf(GREEN);
                                print_delay("\n\n\n\n\n\n\n\n\n\n\n\n Thank you for playing! Goodbye.");
                                clearScreen();
                                printf(RESET);
                                printf("\033[A\033[K");
                                return 0;
                            }
                        }
                        break;
                }
    }
}

// Utility Functions
void clearScreen(void) {
    CLEAR_SCREEN();
}

void display_loading_bar_animated() {
    long delay_ms = (LOADING_TIME_SECONDS * 1000) / TOTAL_ITERATIONS;
    const char* padding = "               ";
    printf("\n\n\n\t\t\t\t\t\t\t\n\n\n\n\n\n\n\n");
    for (int i = 0; i <= TOTAL_ITERATIONS; i++) {
        int key = get_char_no_wait();
        if (key == '\n' || key == '\r') {
            break;
        }

        int filled_chars = (i * BAR_LENGTH) / TOTAL_ITERATIONS;
        int empty_chars = BAR_LENGTH - filled_chars;

        printf("\r%s%s\t\t\t[", padding, CYAN);

        for (int j = 0; j < filled_chars; j++) {
            printf("█");
        }

        printf("%s", RESET);
        for (int j = 0; j < empty_chars; j++) {
            printf("░");
        }
        printf("] ");
        printf("%s%3d%%%s", GREEN, i, RESET);

        fflush(stdout);
        SLEEP_MS(delay_ms);
    }
    printf("\n\n");
    printf(RESET);
    printf("%s%s\t\t\t\t\t >>>>> Loading Complete <<<<<<%s\n\n", YELLOW, padding, RESET);
    SLEEP_MS(1000);
}

static void flush_input(void) {
#ifdef _WIN32
    while (_kbhit()) { (void)getch(); }
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}

static void wait_for_enter(void) {
    printf(YELLOW " Press Enter to continue..." RESET);
#ifdef _WIN32
    int ch = 0;
    while (1) {
        ch = getch();
        if (ch == 13 || ch == '\n' || ch == '\r') break;
    }
#else
    int ch = 0;
    while (1) {
        ch = getch();
        if (ch == '\n' || ch == '\r') break;
    }
#endif
    printf("\n");
}

static int get_char_no_wait() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    struct termios oldt, newt;
    char c;
    ssize_t n;

    tcgetattr(STDIN_FILENO, &oldt); //terminal control, retrieve, attributes; get the current settings of a terminal/tty.
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    n = read(STDIN_FILENO, &c, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (n == 1) {
        return (unsigned char)c;
    } else {
        return -1;
    }
#endif
    return -1;
}

void display_title_screen(int frame) {
    CLEAR_SCREEN();
    char *leftarrow_anim[] = {">",">", " "};
    char *rightarrow_anim[] = {"<","<", " "};

    printf("\n");

    printf("%s\t\t###############################################################################################\n", YELLOW);
    printf("\n");
    printf("%s", GREEN);
    printf(" \t\t  ######    #####   ######    #####   ######   #######  ##   ##  ##   ######   #####   ##    \n");
    printf(" \t\t  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##     \n");
    printf(" \t\t  ######   #######  #######  #######  ##   ##  ##   ##    ###    ##  ##       #######  ##      \n");
    printf(" \t\t  ##       ##   ##  ##  ##   ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##      \n");
    printf(" \t\t  ##       ##   ##  ##   ##  ##   ##  ######   #######  ##   ##  ##   ######  ##   ##  ###### %s\n", RESET);
    printf("\n");
    printf(" %s\t\t\t       %s=================================================================%s\n", CYAN, YELLOW, RESET);
    printf(" %s\t\t\t         >>          T  H  E     A  L  C  H  E  M  I  S  T          <<%s\n",  CYAN, RESET);
    printf(" %s\t\t\t       %s=================================================================%s\n", CYAN, YELLOW, RESET);
    printf("\n");
    printf("\n");
    printf("     \t\t\t\t\t\t %s%s %sPRESS ENTER TO BEGIN%s %s%s\n", leftarrow_anim[frame%3], leftarrow_anim[frame%3],YELLOW, RESET,rightarrow_anim[frame%3],rightarrow_anim[frame%3]);
    printf("\n");

    printf("%s\t\t###############################################################################################\n", YELLOW, RESET);
    printf("\n\n\n");
}
void display_loading_bar() {
    CLEAR_SCREEN();
    long delay_ms = (LOADING_TIME_SECONDS * 1000) / TOTAL_ITERATIONS;
    const char* padding = "               ";
    printf("\n\n\n\t\t\t\t\t\t\t\n\n\n\n\n\n\n\n\n");
    for (int i = 0; i <= TOTAL_ITERATIONS; i++) {
        int key = get_char_no_wait();
        if (key == '\n' || key == '\r') {
            break;
        }
        int filled_chars = (i * BAR_LENGTH) / TOTAL_ITERATIONS;
        int empty_chars = BAR_LENGTH - filled_chars;
        printf("\r\t%s\t\t[%s", padding, CYAN);
        for (int j = 0; j < filled_chars; j++) {
            printf("█");
        }
        printf("%s", RESET);
        for (int j = 0; j < empty_chars; j++) {
            printf("░");
        }
        printf("] ");
        printf("%s%3d%%%s", GREEN, i, RESET);
        fflush(stdout);
        SLEEP_MS(delay_ms);
    }
    printf("\n\n");
    printf(RESET);
    printf("%s%s\t\t\t\t    >>>>>> Loading Complete <<<<<<%s\n\n", YELLOW, padding, RESET);
    SLEEP_MS(1000);
}

void run_loading_sequence() {
    int frame = 0;
    while (1) {
        display_title_screen(frame);
        frame = (frame + 1) % 3;
        SLEEP_MS(300);
        int key = get_char_no_wait();
        if (key == '\n' || key == '\r') {
            break;
        }
    }
    display_loading_bar();
	clearScreen();
    printf ("\n\n\t\t\t"CYAN""BOLD"|------------------------------------------------------------------------------|");
    printf("\n\n\t\t\t\t\t\t     %sWELCOME TO THE GAME!%s\n", YELLOW, RESET );
    printf ("\n\t\t\t"CYAN""BOLD"+------------------------------------------------------------------------------+");
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\t\t\t"RED"   Paradoxical is a contradiction of how the thing you ran away from is the \n\t\t\t thing you must run back to in order to survive...\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[A\033[K");

    clearScreen();
    printf(RESET);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t   You are a quiet but gifted alchemist who abandoned the craft years ago. \n\t\t\t Born in a small mountain village called Dawnhearth, you once studied alchemy\n\t\t\t at Arcanium Lyceum, one of the most prestigious academies in the kingdom.");
    getchar();

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t   You were a prodigy an empyrean until tragedy struck.");
    getchar();

    clearScreen();
    printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t      Your parents both died from the Scarlet Aeonia, a hereditary curse \n\t\t\t    that causes crimson, petal-like lesions to bloom across the body until \n\t\t\t    the victim becomes a withered, unmoving flower.");
    getchar();

    clearScreen();
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t"RED"         Despite all your training, you were too young, too powerless. \n\t\t\t       You could not save them.\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[A\033[K");

    clearScreen();
    printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t"RESET" Disillusioned and heartbroken, you left alchemy behind and returned to\n\t\t       the village farm, quietly living out your days with your younger sister, Malenia.");
    getchar();

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t   One stormy night, lightning cracks across the sky as your sister collapses\n\t\t\t at your doorstep.");
    getchar();

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t       Her pale skin blossoms with deep red petals the same devastating \n\t\t\t     signs you saw on your parents.");
    getchar();

    clearScreen();
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t"RED"      The village healer confirms your worst fear!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[A\033[K");

    clearScreen();
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t   Scarlet Aeonia, the curse has returned.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t"RESET"      There is only one cure, whispered in ancient alchemical tomes's");
    getchar();

    clearScreen();
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t"YELLOW" THE UNALLOYED GOLD NEEDLE!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[A\033[K");

    clearScreen();
    print_delay ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t    a perfect, paradoxical alchemical relic capable of purging corruption!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t  But it can only be forged from five primordial ingredients, each sealed within a\n\t\t\tdungeon tied to the laws of math, logic, and physics challenges deliberately designed by\n\t\t\tancient alchemists to prevent unworthy hands from accessing such power.");
    getchar();

    clearScreen();
    printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t  With Malenia's petals already hardening, time grows short.");
    getchar();

    clearScreen();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t          You retrieve your late masters alchemist kit, dust covering its \n\t\t\tbrass instruments, and make the one choice you swore you would never make again.");
    getchar();

    clearScreen();
}

void display_crafting_title_screen(int frame) {
    CLEAR_SCREEN();
    char *leftarrow_anim[] = {">",">", " "};
    char *rightarrow_anim[] = {"<","<", " "};

    printf("\n");
    printf("%s\t\t\t###############################################################################################\n", CYAN);
    printf("\n");
    printf("%s", YELLOW,BOLD);
    printf(" \t\t\t\t\t   ##    ##  ######  ######  ######   ##      ######       \n");
    printf(" \t\t\t\t\t   ###   ##  ##      ##      ##   ##  ##      ##           \n");
    printf(" \t\t\t\t\t   ####  ##  #####   #####   ##   ##  ##      #####        \n");
    printf(" \t\t\t\t\t   ##  ####  ##      ##      ##   ##  ##      ##           \n");
    printf(" \t\t\t\t\t   ##   ###  ######  ######  ######   ######  ######      %s\n", RESET);
    printf("\n\n");
    printf("%s", YELLOW, BOLD);
    printf(" \t\t\t\t      #####    #####   #####   ##   ##  ####  ######   ######  ######    \n");
    printf(" \t\t\t\t     ##   ##  ##      ##   ##  ##   ##   ##   ##   ##  ##      ##   ##    \n");
    printf(" \t\t\t\t     #######  ##      ##   ##  ##   ##   ##   #######  #####   ##   ##    \n");
    printf(" \t\t\t\t     ##   ##  ##      ##   ##  ##   ##   ##   ##  ##   ##      ##   ##    \n");
    printf(" \t\t\t\t     ##   ##   #####   #####    #####   ####  ##   ##  ######  ######       \n");
    printf(" \t\t\t\t                      ##                                                             %s\n", RESET);
    printf ("\n");

    printf(" %s\t\t\t     %s===============================================================================%s\n", CYAN,CYAN, RESET);
    printf(" %s\t\t\t                         You have successfully completed the game!        \n",  NEON_PINK, RESET);
    printf(" %s\t\t\t                The goal is threaded, a truly pricked and perfect victory! %s\n",  NEON_PINK, RESET);
    printf(" %s\t\t\t     %s===============================================================================%s\n", CYAN, CYAN, RESET);
    printf("\n");

    printf("\n");
    printf("    \t\t\t\t\t\t %s%s %sPRESS ENTER TO SEE NEEDLE CRAFTING%s %s%s\n", leftarrow_anim[frame%3], leftarrow_anim[frame%3], YELLOW, RESET,rightarrow_anim[frame%3],rightarrow_anim[frame%3]);
    printf("\n");

    printf("%s\t\t\t###############################################################################################%s\n", CYAN, RESET);
    printf("\n\n\n");
}

void show_needle_crafting_animation() {
#ifdef _WIN32
    enableANSI();
#endif
    int frame = 0;
    while (1) {
        display_crafting_title_screen(frame);
        frame = (frame + 1) % 3;
        SLEEP_MS(300);

        int key = get_char_no_wait();
        if (key == '\n' || key == '\r') {
            break;
        }
    }

    display_loading_bar_animated();
    CLEAR_SCREEN();

    //cauldron anm
    frame = 0;
    while(1) {
        drawCauldron(frame);
        SLEEP_MS(150);
        frame++;

        int key = get_char_no_wait();
        if (key != -1) {
            break;
        }
    }
    CLEAR_SCREEN();
}

void show_paradoxical(){
    printf(NEON_VIOLET "\n[===========================================================================================================================]\n\n" RESET);
    printf("%s", GREEN);
    printf(" \t\t  ######    #####   ######    #####   ######    #####   ##   ##  ##   ######   #####   ##    \n");
    printf(" \t\t  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##     \n");
    printf(" \t\t  ######   #######  #######  #######  ##   ##  ##   ##    ###    ##  ##       #######  ##      \n");
    printf(" \t\t  ##       ##   ##  ##  ##   ##   ##  ##   ##  ##   ##   # # #   ##  ##       ##   ##  ##      \n");
    printf(" \t\t  ##       ##   ##  ##   ##  ##   ##  ######    #####   ##   ##  ##   ######  ##   ##  ###### %s\n", RESET);
    printf(NEON_VIOLET "\n[===========================================================================================================================]\n\n" RESET);

}

int getValidInt(char *prompt) {
    int value;
    int status;
    char term;
    while(1) {
        printf("%s", prompt);
        status = scanf("%d%c", &value, &term);

        if(status == 2 && term == '\n') {
            return value;
        } else {

            printf(RED "\t\t\t\tInvalid input! Please enter a whole number. (Clearing in 0.5s)\n" RESET);

            if (status != 2 || term != '\n') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
             SLEEP_MS(1000);

            printf("\033[A\033[K");

        }
    }
}


char getBubble(int state) {
    switch (state % 4) {
        case 0: return '.';
        case 1: return 'o';
        case 2: return 'O';
        default: return ' ';
    }
}

void drawCauldron(int frame) {
    CLEAR_SCREEN();

    srand(time(NULL) + frame);

    printf("\n\n\n");
    printf("\t\t\t\t\t%s                                               %s\n", RESET, RESET);
    printf("\t\t\t\t\t%s            %s%c%s %s%c%s %s%c%s\n",
          CYAN, NEON_WHITE, getBubble(rand()), RESET, GREEN, getBubble(rand()), RESET, YELLOW, getBubble(rand()), RESET);
    printf("\t\t\t\t\t%s               %s%c%s %s%c%s %s\n",
           BLUE, NEON_PURPLE, getBubble(rand()), RESET, NEON_AQUA, getBubble(rand()), RESET, RESET);

    printf("\t\t\t\t\t%s         %s/~~/~~\\~~\\%s\n", RESET, GREEN, RESET);
    printf("\t\t\t\t\t%s        /          \\     %s~ Needle Crafting ~%s\n", MAGENTA, ORANGE, RESET);

    printf("\t\t\t\t\t%s       |            |%s\n", GRAY, RESET);
    printf("\t\t\t\t\t%s       |     %s%c%c%c%s    |          %s%c%s%c%s%c%c%c%s\n",
           GRAY, NEON_GOLD, getBubble(frame+1), getBubble(frame+2), getBubble(frame), RESET,
           BLUE, getBubble(frame+3), GREEN, getBubble(frame+4), MAGENTA, getBubble(frame+5), getBubble(frame+6), getBubble(frame+7), RESET);
    printf("\t\t\t\t\t%s        \\     %s%c%c%c%s   /%s\n", GRAY, CYAN, YELLOW, RESET);
    printf("\t\t\t\t\t%s         \\________/%s\n", GRAY, RESET);

    printf("\t\t\t\t\t%s          %s/\\%s/\\%s/\\%s/\\%s\n", RESET, ORANGE, RED, ORANGE, RED, RESET);
    printf("\t\t\t\t\t%s         %s(  >  0  )%s %s\n", ORANGE, RED, RESET, RESET);

    printf("\n\t\t\t\t\t%s╔════════════════════Ingredients════════════════════╗%s\n", NEON_WHITE, RESET);
    printf("\t\t\t\t\t║  %s[1]%s Yellow Ember                                 ║\n", RED, RESET);
    printf("\t\t\t\t\t║  %s[2]%s Arrow of Immunition                          ║\n", YELLOW, RESET);
    printf("\t\t\t\t\t║  %s[3]%s Fire Blossom                                 ║\n", GREEN, RESET);
    printf("\t\t\t\t\t║  %s[4]%s Moon Dewgem                                  ║\n", CYAN, RESET);
    printf("\t\t\t\t\t║  %s[5]%s Blasphemous Claw                             ║\n", BLUE, RESET);
    printf("\t\t\t\t\t╚═══════════════════════════════════════════════════╝%s\n", RESET);

    printf("\n\t\t\t\t\t%sMixing potion...%s\n", CYAN, RESET);
}

float getValidFloat(char *prompt) {
    float value;
    int status;
    char term;
    while(1) {
        printf("%s", prompt);
        status = scanf("%f%c", &value, &term);

        if(status == 2 && term == '\n') {
            return value;
        } else {
            printf(RED "\t\t\t\tInvalid input! Please enter a number. (Clearing in 1.5s)\n" RESET);

            // Clear buffer (sa input valid)
            if (status != 2 || term != '\n') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            SLEEP_MS(1500);
            //erase previous output
            printf("\033[A\033[K");
        }
    }
}

static void render_bar(int current, int max, int width) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;

    int filled = (int)(((double)current / max) * width + 0.5);
    if (filled > width) filled = width;

    putchar('|');
    if (current < max * 0.25) {
        printf(RED);
    } else if (current < max * 0.5) {
        printf(YELLOW);
    } else {
        printf(GREEN);
    }
    for (int i = 0; i < filled; ++i) {
        printf("█");
    }
    printf(RESET);
    for (int i = filled; i < width; ++i) {
        printf("░");
    }
    putchar('|');
}

static void render_bar_exp(int current, int max, int width) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    int filled = (int)(((double)current / max) * width + 0.5);
    if (filled > width) filled = width;
    putchar('|');
    if (current < max * 0.25) {
        printf(BLUE);
    } else if (current < max * 0.5) {
        printf(CYAN);
    } else {
        printf(MAGENTA);
    }
    for (int i = 0; i < filled; ++i) {
        printf("█");
    }
    printf(RESET);
    for (int i = filled; i < width; ++i) {
        printf("░");
    }
    putchar('|');
}

static void render_streak_bar(int current, int max, int width) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    int filled = (int)(((double)current / max) * width + 0.5);
    if (filled > width) filled = width;
    putchar('|');
    printf(YELLOW);
    for (int i = 0; i < filled; ++i) {
        printf("█");
    }
    printf(RESET);
    for (int i = filled; i < width; ++i) {
        printf("░");
    }
    putchar('|');
}

const char* getSpeciesName(Species s) {
    switch(s) {
        case ELF: return "Elf (Mage)";
        case HUMAN: return "Human (Fighter)";
        case GIANT: return "Giant (Tank)";
        default: return "Unknown";
    }
}

void print_delay(const char *message){
    int len = strlen(message);
    for (int i = 0; i < len; i++){
        printf("%c", message[i]);
        fflush(stdout);
        SLEEP_MS(TYPING_SPEED_MS);
    }
    printf("\n");
    wait_for_enter();
}

void displayStats(Player *player) {
    int nextLevelExp = player->level * EXP_THRESHOLD_BASE;
    int toNext = nextLevelExp - player->exp;
    if (toNext < 0) toNext = 0;

    printf(NEON_GOLD "\n[======================================================["NEON_SKY" STATISTICS "NEON_GOLD"]=======================================================]\n");
    printf(NEON_PURPLE " %s " NEON_AQUA "the %s " RESET "\n" BLUE, player->name, getSpeciesName(player->species));
    printf(" LEVEL %d "RESET"" YELLOW "| GOLD: %d" MAGENTA " | ITEMS: P%d E%d\n" RESET,
        player->level, player->gold, player->potions, player->elixirs);

    if (player->saved_state == 1) {
        printf(RED " STATUS: UNSAVED CHANGES\n" RESET);
    } else {
        printf(GREEN " STATUS: SAVED\n" RESET);
    }
    printf(NEON_GOLD "[===========================================================================================================================]\n" RESET);

    printf(GREEN " HP    :" RESET);
    render_bar(player->hp, player->maxHp, 75);
    printf("%d/%d", player->hp, player->maxHp);
    printf("\n\t");
    render_bar(player->hp, player->maxHp, 75);

    printf(ORANGE "\n EXP   :" RESET);
    render_bar_exp(player->exp, nextLevelExp, 75);
    printf("%d/%d (Next: %d)", player->exp, nextLevelExp, toNext);
    printf("\n\t");
render_bar_exp(player->exp, nextLevelExp, 75);

    printf(RED "\n STREAK:" RESET);
    render_streak_bar(player->streak, 5, 75);
    printf("%d/5", player->streak);
    printf("\n\t");
    render_streak_bar(player->streak, 5, 75);

    printf(NEON_GOLD"\n[===========================================================================================================================]\n\n" RESET);
}

//menu w stats
int menu_select(const char *title, const char *options[], int count, Player *player) {
    int selected = 0;
    while (1) {
        clearScreen();
        displayStats(player);
        printf("\n\t\t\t\t       [~~~~~~~~~~~~~~~[ %s ]~~~~~~~~~~~~~~~]\n\n", title);

        for (int i = 0; i < count; ++i) {
            if (i == selected) {
                printf(GREEN "\t\t\t\t\t    >> %s\n" RESET, options[i]);
            } else {
                printf("\t\t\t\t\t       %s\n", options[i]);
            }
        }

        int key = getch();

#ifdef _WIN32
        if (key == 0 || key == 224) {
            int special = getch();
            if (special == 72) { selected = (selected - 1 + count) % count; }
            else if (special == 80) { selected = (selected + 1) % count; }
        } else if (key == 13) { return selected; }

#else
        if (key == 27) {
            int k1 = getch();
            if (k1 == 91) {
                int k2 = getch();
                if (k2 == 65) { selected = (selected - 1 + count) % count; }
                else if (k2 == 66) { selected = (selected + 1) % count; }
            }
        } else if (key == '\n' || key == '\r') { return selected; }
#endif
    }
}

// menu w no stats , PAG START LANG TO
int menu_select_simple(const char *title, const char *options[], int count) {
    int selected = 0;
    clearScreen();
    while (1) {
        clearScreen();
        show_paradoxical();
        printf("\n\n\n\t\t\t\t "RESET" [---------------["NEON_AQUA" %s "RESET"]---------------]\n\n", title);

        for (int i = 0; i < count; ++i) {
            if (i == selected) {
                printf(GREEN "\t\t\t\t\t  >> %s\n" RESET, options[i]);
            } else {
                printf("\t\t\t\t\t     %s\n", options[i]);
            }
        }

        int key = getch();

#ifdef _WIN32
        if (key == 0 || key == 224) {
            int special = getch();
            if (special == 72) { selected = (selected - 1 + count) % count; }
            else if (special == 80) { selected = (selected + 1) % count; }
        } else if (key == 13) { return selected; }

#else
        if (key == 27) {
            int k1 = getch();
            if (k1 == 91) {
                int k2 = getch();
                if (k2 == 65) { selected = (selected - 1 + count) % count; }
                else if (k2 == 66) { selected = (selected + 1) % count; }
            }
        } else if (key == '\n' || key == '\r') { return selected; }
#endif
    }
}

// Create player
void initPlayer(Player *player) {
    clearScreen();
    show_paradoxical();
    //get username
    int valid_name = 0;
    while (!valid_name) {
        printf(MAGENTA "\n Enter your name/username (Max %d characters): " NEON_PINK, NAME_SIZE - 1);
        flush_input();

        // Read the input line
        if (fgets(player->name, NAME_SIZE, stdin) == NULL) {
            strcpy(player->name, "Novice");
            valid_name = 1;
            break;
        }

        // Remove \n sa username
        player->name[strcspn(player->name, "\n")] = 0;
        int is_blank = 1;
        for (int i = 0; player->name[i] != '\0'; i++) {
            if (!isspace(player->name[i])) {
                is_blank = 0;
                break;
            }
        }

        if (is_blank) {
            printf(RED " Name cannot be blank. Please enter a valid name.\n" RESET);
        } else {
            valid_name = 1;
        }
    }

    //Select species
    const char *species_menu[] = {
        " Elf (Mage):"YELLOW" +10% Gold Gain "RESET" ",
        " Human (Fighter):"ORANGE" +10% EXP Gain "RESET" ",
        " Giant (Tank):"RED" +10% Damage Reduction "RESET" ",
    };

    flush_input();
    printf("\n Welcome, %s. Choose your species:\n", player->name);
    int choice = menu_select_simple(" [ SELECT SPECIES ] ", species_menu, 3);
    player->species = (Species)choice;

    //BASE STATS
    player->hp = 100;
    player->maxHp = 100;
    player->gold = 50;
    player->exp = 0;
    player->level = 1;
    player->potions = 0;
    player->elixirs = 0;
    player->vitality_vials = 0;
    player->saved_state = 1;
    player->revival_used = 0;
    player->in_dungeon_seal = 0;
    player->potion_used_in_stage = 0;
    player->streak = 0;

    printf(GREEN "\n\n\n\n\n\n\n\n\n\n\n\n Registration Complete! Welcome, %s the %s.\n" RESET,
        player->name, getSpeciesName(player->species));
    wait_for_enter();
    printf("\033[A\033[K");
}


//File Handling
void saveGame(Player *player) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        const char *message8 = "\n\n\n\n Error saving game.\n";
        printf(RED);
        print_delay(message8);
        printf(RESET);
        printf("\033[A\033[K");
        return;
    }

    time_t timer;
    char date_buffer[DATE_SIZE];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(date_buffer, DATE_SIZE, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(file, "%s\n", date_buffer);
    fprintf(file, "%s\n%d\n", player->name, player->species);
        fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d\n",
            player->hp, player->maxHp, player->gold, player->exp, player->level,
            player->potions, player->elixirs, player->vitality_vials, player->saved_state, player->revival_used, player->streak);
    fclose(file);
    player->saved_state = 0;
}

int loadGame(Player *player, char *date_buffer) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) return 0;

    if (fgets(date_buffer, DATE_SIZE, file) == NULL) {
        fclose(file); return 0;
    }
    date_buffer[strcspn(date_buffer, "\n")] = 0;

    if (fgets(player->name, NAME_SIZE, file) == NULL) {
        fclose(file); return 0;
    }
    player->name[strcspn(player->name, "\n")] = 0;

    int temp_species_int;
    if (fscanf(file, "%d\n", &temp_species_int) != 1) {
        fclose(file); return 0;
    }
    player->species = (Species)temp_species_int;
        int temp_saved_state;
        int temp_revival;
        if (fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d",
            &player->hp, &player->maxHp, &player->gold, &player->exp, &player->level,
            &player->potions, &player->elixirs, &player->vitality_vials, &temp_saved_state, &temp_revival, &player->streak) != 11)
    {
        fclose(file);
        return 0;
    }
        player->saved_state = 0;
        player->revival_used = temp_revival;
    fclose(file);
    return 1;
}
//ask_user | Tutorial
void promptTutorial(Player *player) {
    const char *tutorial_prompt[] = {"Yes, start the guided tutorial", "No, skip and go to the Town Hub"};
    flush_input();
    clearScreen();
    show_paradoxical();
    printf(" Welcome, %s. Would you like a guided introduction to the game?\n", player->name);

    int choice = menu_select(BLUE "TUTORIAL START" RESET, tutorial_prompt, 2, player);
    if (choice == 0) {
        runTutorial(player);
    } else {
        const char *message9 = "\n Tutorial skipped. Entering the Town Hub...";
        printf(YELLOW);
        print_delay(message9);
        printf(RESET);
        printf("\033[A\033[K");

    }
}
// tutorial
void runTutorial(Player *player) {
    clearScreen();
    char *message18 = "\n\n\t\t\t\t  --------------- TUTORIAL STEP 1: STATS & HUD ---------------\n";
    printf(CYAN);
    print_delay(message18);
    printf(RESET);
    printf("\033[A\033[K");
    char *message19 = "\n The game revolves around managing your "GREEN"HP, "YELLOW"Gold, "RESET"and "ORANGE"EXP."RESET"";
    print_delay(message19);
    printf("\033[A\033[K");
    displayStats(player);

    char *message20 = "\n The bars show your progress.\n "GREEN"HP"RESET": Go to the "NEON_LIME"lodge"RESET" or "NEON_LIME"use Potions/Elixirs"RESET" to keep this full.\n "ORANGE"EXP"RESET": Gain this by solving puzzles in the "RED"Dungeon"RESET". Fill the bar to "BLUE"Level Up"RESET"!";
    print_delay(message20);
    printf("\033[A\033[K");

    clearScreen();
    printf(CYAN "\n\n\t\t\t\t  --------------- TUTORIAL STEP 2: HEALING MECHANIC ---------------\n" RESET);
    printf("\n You must manage your HP. Let's practice healing.");

    //get damage
    player->potions++;
    player->hp = player->maxHp / 1.8;
    if (player->hp == 0) player->hp = 1;
    player->saved_state = 1;
    displayStats(player);

    printf(GREEN "\n You received 1 "NEON_LIME"Potion (P)."RESET""GREEN" Your HP has been lowered for practice.\n" RESET);
    wait_for_enter();
    printf("\033[A\033[K");

    // practice heal
    printf(MAGENTA " Now, let's use the "NEON_LIME"Potion!"RESET" (This will not cost a move in the main menu).\n" RESET);
    player->potions--;
    player->hp += POTION_HEAL;
    if (player->hp > player->maxHp) player->hp = player->maxHp;
    player->saved_state = 1;
    clearScreen();
    displayStats(player);
    printf(GREEN "\n Used "NEON_LIME"Potion."RESET""GREEN" HP restored to %d.\n" RESET, player->hp);
    wait_for_enter();
    printf("\033[A\033[K");
    clearScreen();
    printf(CYAN "\n\n\t\t\t\t  --------------- TUTORIAL STEP 3: PUZZLE MECHANIC ---------------\n" RESET);
    printf("\n\t\t\t\tThe main game challenge is the "RED"Dungeon"RESET", where you solve puzzles.\n");
    printf(RED "\t\t\t\tWARNING: In the real Dungeon, failure costs you HP!\n" RESET);
    printf("\t\t\t\tLet's try a simple puzzle now. There are no penalties here.\n\n");

    int success = puzzleLevel1(); // demo puzzle

    int tutorial_gold = 10;
    int tutorial_exp = 10;

    if (success) {
        printf(GREEN "\n\t\t\t\tCorrect! This is what success looks like in the "RED"Dungeon."RESET"\n");
    } else {
        printf(RED "\n\t\t\t\tIncorrect. The correct answer has been revealed for learning." RESET);
        printf(YELLOW "\n\t\t\t\tIn the full game, a "RED"failure"RESET""YELLOW" like this would cost you "GREEN"%d HP."RESET"\n" RESET, BASE_DUNGEON_DAMAGE + (player->level * 2));
    }
    //get reward (starter pack )
    player->gold += tutorial_gold;
    player->exp += tutorial_exp;
    player->saved_state = 1;
    printf(GREEN "\n\t\t\t\tReward granted for tutorial completion:"YELLOW" %d Gold "RESET""GREEN"and"ORANGE" %d EXP.\n\n\n\n\n\n\n\n\n\t\t\t       " RESET, tutorial_gold, tutorial_exp);
    wait_for_enter();
    printf("\033[A\033[K");

    clearScreen();
    printf(CYAN "\n\n\t\t\t\t  --------------- TUTORIAL COMPLETE ---------------\n" RESET);
    printf(" You are now ready to begin your adventure!\n");
    printf(" Remember to Save Game frequently and manage your "GREEN"HP."RESET"\n");
}

//town
void townHub(Player *player) {
    const char *town_menu_lvl1[] = {"General Store", "The Lodge (Heal)", "Library", "Amusement Park (LOCKED)", "Back to Main Menu"};
    const char *town_menu_lvl2[] = {"General Store", "The Lodge (Heal)", "Library", "Amusement Park", "Back to Main Menu"};
    while (1) {
        flush_input();
        int choice;
        if (player->level >= 2) {
            choice = menu_select(BLUE "TOWN HUB" RESET, town_menu_lvl2, 5, player);
        } else {
            choice = menu_select(BLUE "TOWN HUB" RESET, town_menu_lvl1, 5, player);
        }

        switch (choice) {
            case 0: generalStore(player); break;
            case 1: lodge(player); break;
            case 2: library(player); break;
            case 3:
                if (player->level >= 2) {
                    amusementPark(player);
                } else {
                    const char *message = ""RED"\n\n\n\n You must be level 2 or higher to enter the Amusement Park.\n"RESET"";
                    print_delay(message);
                    printf("\033[A\033[K");
                }
                break;
            case 4: return;
        }
    }
}

void generalStore(Player *player) {
    int quantity;
    while (1) {
        clearScreen();
        displayStats(player);
        printf("\n\t\t\t[~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[" MAGENTA " GENERAL STORE " RESET "]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]\n" RESET);
        printf("\t\t\t"GREEN"  BUY:\n");
        printf("\t\t\t  "NEON_LIME"   Potion "GREEN"(Heals %d HP)"RESET" \t\t\t\t"YELLOW" %d  Gold\n", POTION_HEAL, POTION_PRICE);
        printf("\t\t\t  "NEON_LIME"   Elixir "GREEN"(Heals FULL HP)"RESET"\t\t\t\t"YELLOW" %d Gold\n", ELIXIR_PRICE);
        printf("\t\t\t  "NEON_GOLD"   Vial of Vitality "GREEN"(PERMANENT + %d  MAX HP)"YELLOW"  \t %d Gold  "RESET"\n", VIAL_HP_BOOST, VITALITY_VIAL_PRICE);
        printf("\t\t\t"RED"  SELL:\n");
        printf("\t\t\t  "NEON_LIME"   Potion \t\t\t\t\t\t"YELLOW" %d  Gold\n", POTION_SELL_PRICE);
        printf("\t\t\t  "NEON_LIME"   Elixir \t\t\t\t\t\t"YELLOW" %d  Gold\n"RESET"", ELIXIR_SELL_PRICE);
        printf("\t\t\t[~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]\n\n\n\n\n\n\n\n\n");
        wait_for_enter();
        printf("\033[A\033[K");

        const char *store_menu[] = {" "GREEN"Buy "NEON_LIME"Potion", ""GREEN" Buy "NEON_LIME"Elixir", ""GREEN" Buy "NEON_GOLD"Vial of Vitality", ""RED" Sell "NEON_LIME"Potion", ""RED" Sell "NEON_LIME"Elixir", ""GREEN" Back "};
        flush_input();
        int choice = menu_select(ORANGE "Select Transaction" RESET, store_menu, 6, player);

        if (choice == 5) return;

        if (choice == 0 || choice == 1 || choice == 2) {
            quantity = getValidInt(" How many to buy? ");
            if (quantity <= 0) break;
            int price = (choice == 0) ? POTION_PRICE : (choice == 1) ? ELIXIR_PRICE : VITALITY_VIAL_PRICE;

            if (quantity * price <= player->gold) {
                player->gold -= quantity * price;
                if (choice == 0) player->potions += quantity;
                else if (choice == 1) player->elixirs += quantity;
                else if (choice == 2) {
                    player->vitality_vials += quantity;
                    player->maxHp += VIAL_HP_BOOST * quantity;
                    player->hp = player->maxHp;
                    printf(GREEN " PERMANENT UPGRADE! Max HP is now %d.\n" RESET, player->maxHp);
                }
                player->saved_state = 1;
                const char *message10 = " Transaction successful.\n";
                printf(GREEN);
                print_delay(message10);
                printf(RESET);
                printf("\033[A\033[K");
            } else {
                const char *message11 = " Not enough gold to complete transaction.\n";
                printf(RED);
                print_delay(message11);
                printf(RESET);
                printf("\033[A\033[K");
            }
        } else if (choice == 3 || choice == 4) {
            int *item_count = (choice == 3) ? &player->potions : &player->elixirs;
            int sell_price = (choice == 3) ? POTION_SELL_PRICE : ELIXIR_SELL_PRICE;

            quantity = getValidInt(" How many to sell? ");
            if (quantity <= 0) break;

            if (quantity <= *item_count) {
                *item_count -= quantity;
                player->gold += quantity * sell_price;
                player->saved_state = 1;
                printf(GREEN " Sold %d items for %d gold.\n" RESET, quantity, quantity * sell_price);
            } else {
                printf(RED " Not enough items to sell.\n" RESET);
            }
        }

    }
}


void lodge(Player *player) {
    clearScreen();
    displayStats(player);
    printf("\n\n\t\t\t\t\t    [----------["NEON_LIME" THE LODGE "RESET"]----------]\n\n" RESET);
    printf("\n\t\t\t\t\t"NEON_LIME" Resting costs %d Gold and restores full HP."RESET"\n", LODGE_REST_COST);
    printf("\n\t\t\t\t\t [========================================]\n\n\n\n\n\n\n\n\n\n\n\n");
    wait_for_enter();
    printf("\033[A\033[K");
    const char *lodge_menu[] = {"Rest", "Back"};
    flush_input();
    int choice = menu_select(ORANGE "Choose Action" RESET, lodge_menu, 2, player);
    if (choice == 0) {
        if (player->hp == player->maxHp) {
            printf(YELLOW " You are already at full HP!\n" RESET);
        } else if (player->gold >= LODGE_REST_COST) {
            player->gold -= LODGE_REST_COST;
            player->hp = player->maxHp;
            player->saved_state = 1;
            printf(GREEN " You slept soundly. HP restored to %d!\n" RESET, player->maxHp);
        } else {
            printf(RED " Not enough gold to pay the Lodge Keeper.\n" RESET);
        }
    }
}


void showLorePages(const char* book_title, const LoreEntry entries[], int count) {
    int current_page = 0;
    while (1) {
        clearScreen();
        printf(CYAN "\n--- %s ---\n\n" RESET, book_title);

        // Display current page
        printf("%s%s\n" RESET, entries[current_page].color, entries[current_page].title);
        if (entries[current_page].subtitle) {
            printf("%s\n", entries[current_page].subtitle);
        }
        printf("%s\n\n", entries[current_page].description);

        // Display navigation instructions
        printf(GRAY "Use Left/Right arrows to turn pages. Press Enter to exit.\n" RESET);
        printf(YELLOW "Page %d of %d\n" RESET, current_page + 1, count);

        int key = getch();
#ifdef _WIN32
        if (key == 0 || key == 224) { // Arrow keys
            int special = getch();
            if (special == 75) { // Left arrow
                current_page = (current_page - 1 + count) % count;
            } else if (special == 77) { // Right arrow
                current_page = (current_page + 1) % count;
            }
        } else if (key == 13) { // Enter
            break;
        }
#else // macOS
        if (key == 27) { // Escape
            if (getch() == 91) {
                int arrow = getch();
                if (arrow == 68) { // Left arrow
                    current_page = (current_page - 1 + count) % count;
                } else if (arrow == 67) { // Right arrow
                    current_page = (current_page + 1) % count;
                }
            }
        } else if (key == '\n' || key == '\r') { // Enter
            break;
        }
#endif
    }
}


void showIngredientsLore() {
    const LoreEntry ingredient_lore[] = {
        {"Prism Heart", BOLD YELLOW, "A crystalline core that refracts all possible outcomes. It gives the needle precision — so it targets the exact point where a fate can be mended, not just poked at random.", NULL},
        {"Scarlet Emberloom", BOLD RED, "A persistent, heat-generating flower. It’s the forging fire that fuses the ingredients together, and leaves the needle with enough durability to hold up under pressure.", NULL},
        {"Golden Nugget", BOLD NEON_GOLD, "The base material for the needle’s shaft and tip. Malleable enough to shape into a fine point, strong enough to pierce through anything.", NULL},
        {"Inertial Ring", BOLD GRAY, "A solid band that resists external forces. Wrapped around the needle’s base, it stops the stitch from shifting or unraveling once placed.", NULL},
        {"Blasphemous Claws", BOLD MAGENTA, "A sharp fragment from a creature that challenged cosmic rules. When ground down, it lets the needle undo fates that were supposed to be permanent.", NULL}
    };
    showLorePages("Ingredients Meaning", ingredient_lore, 5);
}


void showDungeonsLore() {
    const LoreEntry dungeon_lore[] = {
        {
            "1. The Geometry Gate — “Polyhedra” (Gatewarden)", BOLD,
            "Your journey begins at a colossal gate formed from shifting triangles, squares, and polyhedra — its architecture rearranges like a living puzzle. At the center stands Polyhedra, a being of perfect shape and razor symmetry. To pass, you must calculate the perimeter from the given length and width: correct, and the way opens; wrong, and it tears at your flesh.",
            "Ingredient: " YELLOW "PRISM HEART" RESET
        },
        {
            "2. ETHER-ARC FIELD — “Zephyr” (THE BIRD OF STEADY FLIGHT)", BOLD,
            "Beyond the gate stretches a plain laced with silver ether-threads: perfect arcs, velocity vectors, moon-glow target marks. Step off the weave, and wind hurls you into a lawless path. From high above glides Zephyr — a massive bird with wings of solidified wind and talons like polished arrowheads. Its flight follows perfect projectile motion. To claim the flower, you must calculate time from given distance and velocity; fail, and you become target practice.",
            "Ingredient: " RED "SCARLET EMBERBLOOM" RESET
        },
        {
            "3. Bridge of Pythagoras — The Triad Trolls", BOLD,
            "A vast chasm is bridged only by a narrow stone walkway inscribed with right triangles and a² + b² = c². Upon it stand the Triad Trolls, who demand you solve Pythagorean puzzles to cross. The path requires balance: given sides a and b, you must find c for the numbers to sing in harmony.",
            "Ingredient: " NEON_GOLD "GOLDEN NUGGET" RESET
        },
        {
            "4. Tunnel of Boulders — Reiner (The Inertial Titan)", BOLD,
            "Deep within a mountain lies a tunnel maze etched with acceleration and force equations. Boulders crash down, rocks shift beneath your feet. At the deepest cavern stands Reiner, a stone giant who moves faster the longer he charges. Every stone follows a rule — learn it, or be crushed. To pass, you must calculate acceleration from given force and mass.",
            "Ingredient: " GRAY "INERTIAL RING" RESET
        },
        {
            "5. Magister's Cave — The Boolean Beast", BOLD,
            "Your path leads to a broken cliffside laboratory filled with cracked vials, scattered notes, and ruined experiments. At the cave's heart crouches the Boolean Beast — a twisted mass of shifting logic, once a daring magister. To claim its treasure, you must arrange given numbers into flawless patterns; fail, and your mind short-circuits into endless contradiction.",
            "Ingredient: " MAGENTA "BLASPHEMOUS CLAW" RESET
        }
    };
    showLorePages("The Five Dungeons of Formulaic Creation", dungeon_lore, 5);
}


void library(Player *player) {
    const char *library_menu[] = {"Ingredients Lore", "Dungeon Lore",  "Back"};
    while (1) {
        flush_input();
        int choice = menu_select(CYAN "LIBRARY" RESET, library_menu, 3, player);
        switch (choice) {
            case 0: showIngredientsLore(); break;
            case 1: showDungeonsLore(); break;
            case 2: return;
        }
    }
}


int useItem(Player *player) {
    char item_menu_1[60];
    char item_menu_2[60];
    //limit potion

    while (1) {
        sprintf(item_menu_1, "Use Potion (50 HP) (" YELLOW "%d available" RESET ")", player->potions);
        sprintf(item_menu_2, "Use Elixir (Full HP) (" YELLOW "%d available" RESET ")", player->elixirs);
        const char *item_menu[] = {item_menu_1, item_menu_2, "Back"};
        flush_input();
        int choice = menu_select(MAGENTA "USE ITEM" RESET, item_menu, 3, player);
        if (choice == 2) return 0;
        int used = 0;
        if (choice == 0) {
            if (player->potions > 0 && player->hp < player->maxHp) {
                if (player->in_dungeon_seal && player->potion_used_in_stage) {
                    printf(YELLOW " Cannot use: " RESET);
                    printf(YELLOW " You already used a Potion during this dungeon stage." RESET);
                } else {
                    player->potions--;
                    player->hp += POTION_HEAL;
                    if (player->hp > player->maxHp) player->hp = player->maxHp;
                    player->saved_state = 1;
                    printf(GREEN " Used Potion. HP restored to %d.\n" RESET, player->hp);
                    used = 1;
                    if (player->in_dungeon_seal) player->potion_used_in_stage = 1;
                }
            } else {
                printf(YELLOW " Cannot use: " RESET);
                if (player->potions == 0) printf(RED " No potions." RESET);
                else printf(YELLOW " Already full HP." RESET);
            }
        } else if (choice == 1) {
            //block elixir
            if (player->in_dungeon_seal) {
                printf(YELLOW " Cannot use Elixir: " RESET);
                printf(YELLOW " Elixirs cannot be used while sealed in combat.\n" RESET);
            } else if (player->elixirs > 0 && player->hp < player->maxHp) {
                player->elixirs--;
                player->hp = player->maxHp;
                player->saved_state = 1;
                printf(GREEN " Used Elixir. HP fully restored to %d.\n" RESET, player->maxHp);
                used = 1;
            } else {
                printf(YELLOW " Cannot use: " RESET);
                if (player->elixirs == 0) printf(RED " No elixirs." RESET);
                else printf(YELLOW " Already full HP." RESET);
            }
        }
        if (used) {
            wait_for_enter();
            printf("\033[A\033[K");
            return 1; // Return to the main menu
        }
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return 0;
}

void applySpeciesBonus(Player *player, int *expGain, int *goldGain) {
    if (player->species == ELF) {
        *goldGain = (int)(*goldGain * 1.10);   // +10% gold
    }
    else if (player->species == HUMAN) {
        *expGain = (int)(*expGain * 1.10);     // +10% exp
    }
    else if (player->species == GIANT) {
        player->maxHp = (int)(player->maxHp * 1.10);   // +10% max HP
        if (player->hp > player->maxHp) player->hp = player->maxHp;
    }
}

int levelUp(Player *player) {
    int threshold = player->level * EXP_THRESHOLD_BASE;
    int leveled_up = 0;
    while (player->exp >= threshold) {
        leveled_up = 1;
        player->saved_state = 1;
        player->level++;
        player->maxHp += 20;
        threshold = player->level * EXP_THRESHOLD_BASE;
        printf(YELLOW "\n\t\t\t\t *** LEVEL UP! You are now Level %d! ***\n" RESET, player->level);

        if (player->level == 2) {
            printf(CYAN "\n\t\t\t\t *** NEW FEATURE UNLOCKED: Amusement Park! ***\n" RESET);
            printf("\t\t\t\t You can now visit the Amusement Park from the main menu.\n");
            wait_for_enter();
            printf("\033[A\033[K");
        }

        if (player->level == 6) {
            printf(NEON_VIOLET "\n *** You have reached the pinnacle of your craft! ***\n" RESET);
            show_needle_crafting_animation();
        }

        const char *message12 = "\t\t\t\t Max HP increased by 20!\n";
        printf(GREEN);
        print_delay(message12);
        printf(RESET);
        printf("\033[A\033[K");
    }
    return leveled_up;
}

// Dungeon and Combat
void dungeon(Player *player) {
    int level = 1;
    int first_trial = 1; // check if galing menu
    player->in_dungeon_seal = 1; // true
    while (1) {
        int damage_penalty = BASE_DUNGEON_DAMAGE + (player->level * 2);
        float gold_reward = 10.0;
        int exp_reward = 14;
        player->potion_used_in_stage = 0;
        int retreat_cost = 10 * player->level;
        clearScreen();
        displayStats(player);
        printf(RED "\n\n\t\t\t\t     [---------------[ DUNGEON TRIAL %d ]---------------]\n\n" RESET, level);
        if (first_trial) {
            printf("\t\t\t\t\t   HP Penalty for failure: %d HP\n", damage_penalty);
            printf("\t\t\t\t\t   Reward for success: %.0f Gold & %d EXP\n\n\n\n\n", gold_reward, exp_reward);
            wait_for_enter();
            printf("\033[A\033[K");
            first_trial = 0;
        }
        clearScreen();

        char retreat_option[80];
        sprintf(retreat_option, "Gate keeper's bribe(Cost: " YELLOW "%d Gold" RESET ", 50%% chance to succeed)", retreat_cost);

        const char *dungeon_menu[] = {"Attempt the puzzle", retreat_option, "Use Potion (In-Dungeon)"};
        flush_input();
        int choice = menu_select(RED "DUNGEON ACTION" RESET, dungeon_menu, 3, player);
        clearScreen();

        displayStats(player);
        printf("\n\n");

        if (choice == 1) {
            if (player->gold >= retreat_cost) {
                player->gold -=
                retreat_cost;
                player->saved_state = 1;
                printf(YELLOW "\n You offer %d Gold to bypass the trial...\n" RESET, retreat_cost);

                printf(YELLOW " The Gatekeep considers your offer...\n" RESET);
                SLEEP_MS(1000);
                printf(YELLOW " The tension grows...\n" RESET);
                SLEEP_MS(2500); // 50/50 chance
                if (rand() % 2 == 0) {
                    char *message22 = "\n SUCCESS! The guardian accepts your payment.\n\ You may pass this time.\n You are safely escorted back to town.\n";
                    printf(GREEN);
                    print_delay(message22);
                    printf(RESET);
                    printf("\033[A\033[K");
                    player->in_dungeon_seal = 0; // seal off
                    player->streak = 0;
                    return;
                } else { // Bribe failed
                    char *message23 = "\n BYPASS FAILED! \n I see your bribe, but the dice of fate says NO!\n\n";
                    printf(RED);
                    print_delay(message23);
                    printf(RESET);
                    printf("\033[A\033[K");
                }
            } else {
                printf(RED "\n You don't have enough Gold (%d required) to attempt to try to bypass the puzzle.\n" RESET, retreat_cost);
            }
        }
        else if (choice == 2) {
            // 1 potion per level
            if (player->potions <= 0) {
                printf(YELLOW "\n Cannot use potion: " RESET);
                printf(RED " No potions available.\n" RESET);
                wait_for_enter();
                printf("\033[A\033[K");
                continue;
            }
            if (player->hp >= player->maxHp) {
                printf(YELLOW "\n Already at full HP. No need to use a potion.\n" RESET);
                wait_for_enter();
                printf("\033[A\033[K");
                continue;
            }
            if (player->potion_used_in_stage) {
                printf(YELLOW "\n You have already used a potion during this dungeon stage.\n" RESET);
                wait_for_enter();
                printf("\033[A\033[K");
                continue;
            }
            // Use the potion
            player->potions--;
            player->hp += POTION_HEAL;
            if (player->hp > player->maxHp) player->hp = player->maxHp;
            player->potion_used_in_stage = 1;
            player->saved_state = 1;
            printf(GREEN "\n Used Potion in-dungeon. HP restored to %d.\n" RESET, player->hp);
            wait_for_enter();
            printf("\033[A\033[K");
            continue;
        }
        int success = 0;
        int (*puzzleFuncs[6])(void) = { NULL, puzzleLevel1, puzzleLevel2, puzzleLevel3, puzzleLevel4, puzzleLevel5 };

        int puzzle_index = player->level;
        if (puzzle_index < 1) puzzle_index = 1; // para lang di mag error if ever
        if (puzzle_index > 5) puzzle_index = 5;

        success = puzzleFuncs[puzzle_index]();
        if (success) {
			printf("\n\n");
            printf(GREEN " Correct! Puzzle solved.\n" RESET);
            player->streak++;
            if (player->streak > 5) {
                player->streak = 5;
            }

            int final_gold = (int)gold_reward;
            int final_exp = exp_reward;

            if (player->streak >= 5) {
                final_gold *= GOLD_STREAK;
                final_exp *= EXP_STEAK;
                const char *message13 = "\n *** STREAK BONUS! ***\n";
                printf(YELLOW);
                print_delay(message13);
                printf(RESET);
                printf("\033[A\033[K");
                const char *message14 = " Reward increased!\n";
                printf(GREEN);
                print_delay(message14);
                printf(RESET);
                printf("\033[A\033[K");
            }

            // species bonus
            if (player->species == ELF) {
                final_gold *= 1.10;
                printf(CYAN " (Elf bonus: +10%% Gold reward!)\n" RESET);
            }
            if (player->species == HUMAN) {
                final_exp *= 1.10;
                printf(BLUE " (Human bonus: +10%% EXP reward!)\n" RESET);
            }
            player->gold += final_gold;
            player->exp += final_exp;
            player->saved_state = 1;

            //If true level up
            if (levelUp(player)) {
                player->in_dungeon_seal = 0; // Deactivate Dungeon Seal
                printf(CYAN "\n Level Increased!!.. Returning to main menu.\n" RESET);
                wait_for_enter();
                printf("\033[A\033[K");
                return;
            }
            wait_for_enter();
            printf("\033[A\033[K");
            level++;
        } else {
            if (player->species == GIANT) {
                damage_penalty = (int)(damage_penalty * 0.90); // 10% damage reduction
                printf(RED " WRONG! " RESET);
                printf(MAGENTA "(Giant resilience: Damage reduced by 10%%!) " RESET);
                printf(RED " You take %d damage.\n" RESET, damage_penalty);
            } else {
                printf(RED " WRONG! You take %d damage.\n" RESET, damage_penalty);
            }
            player->streak = 0;
            player->hp -= damage_penalty;
            player->saved_state = 1;

            if (player->hp <= 0) {
                const char *message15 = "\n YOU HAVE FAINTED! The trial ends.\n";
                printf(RED);
                print_delay(message15);
                printf(RESET);
                printf("\033[A\033[K");

                //ask revival (1 time only)
                if (player->revival_used == 0) {
                    const char *revive_menu[] = {"Pay 100 Gold to Revive", "Decline - Reset Game"};
                    flush_input();
                    int revive_choice = menu_select(RED "REVIVE OPTION" RESET, revive_menu, 2, player);

                    if (revive_choice == 0) {
                        if (player->gold >= 100) {
                            player->gold -= 100;
                            player->revival_used = 1;
                            player->hp = player->maxHp / 2;
                            if (player->hp == 0) player->hp = 1;
                            player->saved_state = 1;
                            printf(GREEN " You have been revived at the cost of 100 Gold.\n" RESET);
                            wait_for_enter();
                            printf("\033[A\033[K");
                            player->in_dungeon_seal = 0;
                            return;
                        } else {
                            printf(YELLOW " Not enough Gold to pay for revival. The game will reset.\n" RESET);
                            wait_for_enter();
                            printf("\033[A\033[K");
                            player->in_dungeon_seal = 0;
                            initPlayer(player);
                            return;
                        }
                    } else {
                        //reset
                        printf(YELLOW " You chose not to revive. Resetting the game now.\n" RESET);
                        wait_for_enter();
                        printf("\033[A\033[K");
                        player->in_dungeon_seal = 0;
                        initPlayer(player);
                        return;
                    }
                } else {
                    printf(RED " You have already used your one revival. Resetting the game now.\n" RESET);
                    wait_for_enter();
                    printf("\033[A\033[K");
                    player->in_dungeon_seal = 0;
                    initPlayer(player);
                    return;
                }
            }
        }
    }
}

int puzzleLevel1(void) {
    int l = rand() % 15 + 3;
    int w = rand() % 15 + 3;
    int ansArea, ansPerim;
    int correctArea = l * w;
    int correctPerim = 2 * (l + w);

    printf("\t\t\t\t"BLUE"GEOMETRY GATE:"RESET" A rectangle has Length %d and Width %d.\n", l, w);
    printf("\t\t\t\tFormula: Area = L * W | Perimeter = 2 * (L + W)\n");
    ansArea = getValidInt("\n\n\t\t\t\t1. What is the Area? ");
    ansPerim = getValidInt("\n\t\t\t\t2. What is the Perimeter? ");

    // Check if correct
    int success = (ansArea == correctArea && ansPerim == correctPerim);

    if (!success) {
        printf("\n\n");
        printf(RED "\t\t\t\tWRONG!\n" RESET);
        printf(GREEN "\t\t\t\tThe correct answers were:\n" RESET);
        printf("\t\t\t\tArea: %d\n", correctArea);
        printf("\t\t\t\tPerimeter: %d\n", correctPerim);
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return success;
}

int puzzleLevel2(void) {
    int dist = (rand() % 10 + 5) * 10;
    int vel = rand() % 5 + 5;
    float ansTime;
    float correctTime = (float)dist / vel;
    printf("\t\t\t\t"NEON_PURPLE"SKELETON ARCHER:"RESET" Target is %dm away. Arrow speed is %dm/s.\n", dist, vel);
    printf("\t\t\t\tFormula: Time = Distance / Velocity\n");
    ansTime = getValidFloat("\n\n\t\t\t\tHow many seconds to hit (to 2 decimal places)? ");

    int success = (fabs(ansTime - correctTime) < 0.01);
    if (!success) {
        printf("\n\n");
        printf(RED "\t\t\t\tWRONG!\n" RESET);
        printf(CYAN "\t\t\t\tThe correct answer was: %.2f seconds\n\t\t\t\t" RESET, correctTime);
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return success;
}

int puzzleLevel3(void) {
    int a = rand() % 10 + 3;
    int b = rand() % 10 + 3;
    float ansHyp;
    float correctHyp = sqrt((a*a) + (b*b));
    printf("\t\t\t\t"MAGENTA"BROKEN BRIDGE:"RESET" Right triangle with legs a=%d, b=%d.\n", a, b);
    printf("\t\t\t\tFormula: c = sqrt(a^2 + b^2)\n");
    ansHyp = getValidFloat("\n\n\t\t\t\tCalculate hypotenuse c (to 2 decimal places): ");

    int success = (fabs(ansHyp - correctHyp) < 0.01);
    if (!success) {
        printf("\n\n");
        printf(RED "\t\t\t\tWRONG!\n" RESET);
        printf(CYAN "\t\t\t\tThe correct answer was: %.2f\n\t\t\t\t" RESET, correctHyp);
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return success;
}

int puzzleLevel4(void) {
    int force = (rand() % 50 + 10) * 10;
    int mass = rand() % 10 + 5;
    float ansAcc;
    float correctAcc = (float)force / mass;

    printf("\t\t\t\t"ORANGE"GOLEM BOULDER:"RESET" Mass is %dkg. Force applied is %dN.\n", mass, force);
    printf("\t\t\t\tFormula: Acceleration = Force / Mass\n");

    ansAcc = getValidFloat("\n\n\t\t\t\tCalculate Acceleration (m/s^2, to 2 decimal places): ");

    int success = (fabs(ansAcc - correctAcc) < 0.01);
    if (!success) {
        printf("\n\n");
        printf(RED "\t\t\t\tWRONG!\n" RESET);
        printf(CYAN "\t\t\t\tThe correct answer was: %.2f m/s^2\n\t\t\t\t" RESET, correctAcc);
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return success;
}

int puzzleLevel5(void) {
    int sequence[5];
    int correct_answer;
    int user_answer;
    const char *puzzle_name;
    const char *formula_hint;

    int pattern_type = rand() % 3; //0-2

    switch (pattern_type) {
        case 0: { // Arithmetic
            puzzle_name = "Sequence of the Constant Step";
            formula_hint = "Each number increases by the same amount.";
            int start_num = rand() % 10 + 1;
            int diff = rand() % 8 + 2;//2-9
            sequence[0] = start_num;
            for (int i = 1; i < 5; i++) {
                sequence[i] = sequence[i-1] + diff;
            }
            correct_answer = sequence[4] + diff;
            break;
        }
        case 1: { // Geometric
            puzzle_name = "Sequence of Exponential Growth";
            formula_hint = "Each number is multiplied by the same factor.";
            int start_num = rand() % 4 + 2;
            int ratio = rand() % 2 + 2; //
            sequence[0] = start_num;
            for (int i = 1; i < 5; i++) {
                sequence[i] = sequence[i-1] * ratio;
            }
            correct_answer = sequence[4] * ratio;
            break;
        }
        case 2: { // Second-Order Arithmetic
            puzzle_name = "Sequence of the Growing Step";
            formula_hint = "The amount added increases each time.";
            int start_num = rand() % 5 + 1;
            int initial_diff = rand() % 3 + 2;
            int diff_increase = rand() % 3 + 1;
            sequence[0] = start_num;
            int current_diff = initial_diff;
            for (int i = 1; i < 5; i++) {
                sequence[i] = sequence[i-1] + current_diff;
                current_diff += diff_increase;
            }
            correct_answer = sequence[4] + current_diff;
            break;
        }
    }

    printf("\t\t\t\t"NEON_AQUA" LICH OF LOGIC:"RESET" The final trial is the **%s**.\n", puzzle_name);
    printf("\t\t\t\t Observe the pattern and determine the next number.\n" GRAY "\t\t\t\t (Hint: %s)\n\n" RESET, formula_hint);

    printf("\t\t\t\t Sequence: [ ");
    for (int i = 0; i < 5; i++) {
        printf(YELLOW "%d" RESET, sequence[i]);
        if (i < 4) printf(", ");
    }
    printf(" ]\n");

    user_answer = getValidInt("\n\n\t\t\t\t What is the next term in the sequence? ");

    int success = (user_answer == correct_answer);
    if (!success) {
        printf("\n\n");
        printf(RED "\t\t\t\t WRONG!\n" RESET);
        printf(CYAN "\t\t\t\t The correct answer was: %d\n" RESET, correct_answer);
        wait_for_enter();
        printf("\033[A\033[K");
    }
    return success;
}

void guessTheNumberGame(Player *player) {
    const int TICKET_PRICE = 15;
    const int MAX_GUESSES = 6;
    const int REWARD = 50;
    clearScreen();
    displayStats(player);
    printf("\n\t\t\t\t     " CYAN "~~~~~~~~~~~~~~~[ GUESS THE NUMBER ]~~~~~~~~~~~~~~~" RESET "\n");
    printf("\n\t\t\t\tWelcome! Try to guess the secret number between 1 and 100.\n");
    printf("\t\t\t\tAfter each guess, I'll tell you if your guess was too high or too low.\n");
    printf("\t\t\t\tIt costs " YELLOW "%d Gold" RESET " to play. You have %d guesses.\n", TICKET_PRICE, MAX_GUESSES);
    printf("\t\t\t\tIf you win, you get " YELLOW "%d Gold" RESET "!\n\n\n\n\n\n\n\n\n\n\n\n", REWARD);

    if (player->gold < TICKET_PRICE) {
        printf(RED "\t\t\t\tYou don't have enough gold to play.\n\t\t\t\t" RESET);
        wait_for_enter();
        printf("\033[A\033[K");
        return;
    }

    wait_for_enter();
    printf("\033[A\033[K");

    const char *play_menu[] = {"Pay and Play", "Maybe later"};
    int choice = menu_select("Ready to play?", play_menu, 2, player);
    if (choice == 1) {
        return;
    }

    player->gold -= TICKET_PRICE;
    player->saved_state = 1;
    int secret_number = (rand() % 100) + 1;
    int guesses_left = MAX_GUESSES;
    int player_guess;

    while (guesses_left > 0) {
        clearScreen();
        displayStats(player);
        printf("\n\t\t\t\t     " CYAN "~~~~~~~~~~~~~~~[ GUESS THE NUMBER ]~~~~~~~~~~~~~~~" RESET "\n");
        printf("\n\t\t\t\t        Guesses left: " YELLOW "%d" RESET, guesses_left);

        player_guess = getValidInt("\n\n\t\t\t\t        Enter your guess (1-100): ");
        if (player_guess == secret_number) {
            printf(GREEN "\n\t\t\t\t Congratulations! You guessed the number %d!\n" RESET, secret_number);
            printf("\t\t\t\t You win " YELLOW "%d Gold" RESET "!\n", REWARD);
            player->gold += REWARD;
            player->saved_state = 1;
            wait_for_enter();
            printf("\033[A\033[K");
            return;
        } else if (player_guess < secret_number) {
            printf(YELLOW "\n\t\t\t\t        Too low! Try a higher number.\n" RESET);
        } else {
            printf(YELLOW "\n\t\t\t\t        Too high! Try a lower number.\n" RESET);
        }
        guesses_left--;
        wait_for_enter();
        printf("\033[A\033[K");
    }
    printf(RED "\n\t\t\t\t        Oh no! You've run out of guesses. The secret number was %d.\n" RESET, secret_number);
    printf("\t\t\t\t        Better luck next time!\n");
    wait_for_enter();
    printf("\033[A\033[K");
}

void amusementPark(Player *player) {
    const char *park_menu[] = {"Play 'Guess the Number'", "Back to Main Menu"};
    while (1) {
        flush_input();
        int choice = menu_select(CYAN "AMUSEMENT PARK" RESET, park_menu, 2, player);
        switch (choice) {
            case 0:
                guessTheNumberGame(player);
                break;
            case 1:
                return;
        }
    }
}
