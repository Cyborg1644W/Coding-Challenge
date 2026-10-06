#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// --- ANSI Color Codes for Interface Polish (20% Grade) ---
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

// --- CONSTANTS ---
#define FILE_NAME "alchemist_save.dat" // Standardized save file name
#define EXP_THRESHOLD_BASE 100 

// Inventory & Cost Constants
#define POTION_PRICE 20
#define ELIXIR_PRICE 50
#define VITALITY_VIAL_PRICE 150 // New Item for Max HP
#define POTION_SELL_PRICE 10
#define ELIXIR_SELL_PRICE 25
#define POTION_HEAL 50
#define INN_REST_COST 30
#define VIAL_HP_BOOST 10

// Dungeon Constant - Damage now scales with level
#define BASE_DUNGEON_DAMAGE 10

#ifdef _WIN32
#include <conio.h>
#else
// POSIX fallback for non-blocking single-key input (required for arrow menu)
#include <termios.h>
#include <unistd.h>
static int getch_posix(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#define getch() getch_posix()
#endif

// --- STRUCT (MANDATORY POINTER USAGE) ---
// Using a struct allows passing all state variables efficiently via a single pointer.
typedef struct {
    int hp;
    int maxHp;
    int gold;
    int exp;
    int level;
    int potions;  // Inventory: number of potions
    int elixirs;  // Inventory: number of elixirs
    int vitality_vials; // New: Vials purchased
} Player;

// --- UTILITY PROTOTYPES ---
void clearScreen();
static void flush_input(void);
static void wait_for_enter(void);
int menu_select(const char *title, const char *options[], int count, Player *player);
void displayStats(Player *player);

// Robust input handlers (Fix: Prevent crashes from non-numeric input)
int getValidInt(char *prompt); 
float getValidFloat(char *prompt);

// --- GAME LOGIC PROTOTYPES ---
void saveGame(Player *player);
int loadGame(Player *player);
void townHub(Player *player);
void generalStore(Player *player);
void inn(Player *player);
void dungeon(Player *player);
void levelUp(Player *player);
void useItem(Player *player);

// Puzzle Prototypes (Minimum 5 required)
int puzzleLevel1(); 
int puzzleLevel2(); 
int puzzleLevel3(); 
int puzzleLevel4(); 
int puzzleLevel5(); // Array Sorting Puzzle

// --- MAIN GAME LOOP ---
int main() {
    Player player;
    srand(time(NULL));

    clearScreen();
    printf(CYAN "========================================\n");
    printf("     THE ALCHEMIST'S TRADE & TRIALS\n");
    printf("========================================\n" RESET);
    printf("Tip: Use UP / DOWN arrow keys to move the menu cursor, then press Enter to select.\n\n");

    // File Handling: Load game or initialize new state
    if (!loadGame(&player)) {
        player.hp = 100;
        player.maxHp = 100;
        player.gold = 50;
        player.exp = 0;
        player.level = 1;
        player.potions = 0;
        player.elixirs = 0;
        player.vitality_vials = 0;
        printf(YELLOW "Starting a new adventure as a Novice Alchemist!\n" RESET);
    } else {
        printf(GREEN "Save file loaded successfully! Welcome back.\n" RESET);
    }

    wait_for_enter();

    const char *main_menu[] = {"Visit Town Hub", "Enter Dungeon", "Use Item", "Save Game", "Exit"};
    
    // Loops: Main Game Loop
    while (1) {
        flush_input(); // Clear input buffer before menu read
        int sel = menu_select(CYAN "MAIN MENU" RESET, main_menu, 5, &player);
        
        // Conditionals: Switch for menu selection
        switch (sel) {
            case 0:
                townHub(&player);
                break;
            case 1:
                if (player.hp > 0) {
                    dungeon(&player);
                } else {
                    printf(RED "\nYou are too weak! Visit the Inn or use an item first.\n" RESET);
                    wait_for_enter();
                }
                break;
            case 2:
                useItem(&player);
                break;
            case 3:
                saveGame(&player);
                printf(GREEN "Game saved successfully!\n" RESET);
                wait_for_enter();
                break;
            case 4:
                printf(CYAN "Thank you for playing! Goodbye.\n" RESET);
                return 0;
        }
    }
}

// --- UTILITY FUNCTIONS ---

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void flush_input(void) {
#ifdef _WIN32
    while (_kbhit()) { (void)getch(); }
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}

static void wait_for_enter(void) {
    printf(YELLOW "Press Enter to continue..." RESET);
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

// Robust Input Validation for Integers (Fixes non-numeric crash bug)
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
            printf(RED "Invalid input! Please enter a whole number.\n" RESET);
            if (status != 2) while(getchar() != '\n'); // Clear buffer if scanf failed before reading term
        }
    }
}

// Robust Input Validation for Floats (Fixes non-numeric crash bug)
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
            printf(RED "Invalid input! Please enter a number.\n" RESET);
            if (status != 2) while(getchar() != '\n');
        }
    }
}


// Renders a visual progress bar with color-coding (UX Improvement)
static void render_bar(int current, int max, int width) {
    if (max <= 0) max = 1;
    if (current < 0) current = 0;
    
    int filled = (int)(((double)current / max) * width + 0.5);
    if (filled > width) filled = width;

    putchar('[');
    // Dynamic HP Bar Color (UX Improvement)
    if (current < max * 0.25) {
        printf(RED);
    } else if (current < max * 0.5) {
        printf(YELLOW);
    } else {
        printf(GREEN);
    }
    
    for (int i = 0; i < filled; ++i) {
        putchar('#');
    }
    
    printf(RESET);
    for (int i = filled; i < width; ++i) {
        putchar('-');
    }
    putchar(']');
}

// Displays player stats at the top of every menu (UX Improvement)
void displayStats(Player *player) {
    int nextLevelExp = player->level * EXP_THRESHOLD_BASE;
    int toNext = nextLevelExp - player->exp;
    if (toNext < 0) toNext = 0;

    printf(MAGENTA "========================================\n");
    printf(" LEVEL %d | GOLD: " YELLOW "%d" MAGENTA " | ITEMS: P%d E%d\n" RESET, 
        player->level, player->gold, player->potions, player->elixirs);
    printf(MAGENTA "========================================\n" RESET);
    
    // HP visual bar
    printf("HP:  ");
    render_bar(player->hp, player->maxHp, 25);
    printf(" %d/%d\n", player->hp, player->maxHp);

    // EXP visual bar
    printf("EXP: ");
    render_bar(player->exp, nextLevelExp, 25);
    printf(" %d/%d (Next: %d)\n", player->exp, nextLevelExp, toNext);

    printf(MAGENTA "========================================\n" RESET);
}

// The core arrow-key menu logic
int menu_select(const char *title, const char *options[], int count, Player *player) {
    int selected = 0;
    while (1) {
        clearScreen();
        displayStats(player); // Show stats automatically (UX Improvement)
        printf("\n--- %s ---\n", title);

        for (int i = 0; i < count; ++i) {
            if (i == selected) {
                printf(GREEN " > %s\n" RESET, options[i]);
            } else {
                printf("   %s\n", options[i]);
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
                if (k2 == 65) { selected = (selected - 1 + count) % count; } // up (A)
                else if (k2 == 66) { selected = (selected + 1) % count; } // down (B)
            }
        } else if (key == '\n' || key == '\r') { return selected; } 
#endif
    }
}

// --- FILE HANDLING (Mandatory Requirement) ---

void saveGame(Player *player) {
    FILE *file = fopen(FILE_NAME, "w"); // Using standardized FILE_NAME
    if (file == NULL) {
        printf(RED "Error saving game.\n" RESET);
        return;
    }
    // Saving all stats including the new vitality_vials
    fprintf(file, "%d %d %d %d %d %d %d %d\n", 
            player->hp, player->maxHp, player->gold, player->exp, player->level, 
            player->potions, player->elixirs, player->vitality_vials);
    fclose(file);
}

int loadGame(Player *player) {
    FILE *file = fopen(FILE_NAME, "r"); // Using standardized FILE_NAME
    if (file == NULL) return 0;
    
    // Reading all stats, checking for 8 successful reads
    if (fscanf(file, "%d %d %d %d %d %d %d %d", 
               &player->hp, &player->maxHp, &player->gold, &player->exp, &player->level, 
               &player->potions, &player->elixirs, &player->vitality_vials) != 8) 
    {
        fclose(file);
        return 0; // Corrupt or old file format
    }
    fclose(file);
    return 1;
}

// --- TOWN HUB MODULE ---

void townHub(Player *player) {
    const char *town_menu[] = {"General Store", "The Inn (Heal)", "Back to Main Menu"};
    while (1) {
        flush_input();
        int choice = menu_select(BLUE "TOWN HUB" RESET, town_menu, 3, player);
        switch (choice) {
            case 0: generalStore(player); break;
            case 1: inn(player); break;
            case 2: return;
        }
    }
}

// 2. Business Module: General Store
void generalStore(Player *player) {
    int quantity;
    while (1) {
        clearScreen();
        displayStats(player);
        printf("\n" BLUE "--- GENERAL STORE ---\n" RESET);
        printf("BUY:\n");
        printf(" 1. Potion (Heals %d HP) - %d Gold\n", POTION_HEAL, POTION_PRICE);
        printf(" 2. Elixir (Heals FULL HP) - %d Gold\n", ELIXIR_PRICE);
        printf(" 3. Vial of Vitality (PERMANENT +" RED "%d" BLUE " MAX HP) - %d Gold\n" RESET, VIAL_HP_BOOST, VITALITY_VIAL_PRICE); // NEW ITEM
        printf("SELL:\n");
        printf(" 4. Potion - %d Gold\n", POTION_SELL_PRICE);
        printf(" 5. Elixir - %d Gold\n", ELIXIR_SELL_PRICE);
        printf("---------------------\n");
        
        const char *store_menu[] = {"Buy Potion", "Buy Elixir", "Buy Vial of Vitality", "Sell Potion", "Sell Elixir", "Back"};
        flush_input();
        int choice = menu_select("Select Transaction", store_menu, 6, player);

        if (choice == 5) return;

        if (choice == 0 || choice == 1 || choice == 2) { // BUY LOGIC
            quantity = getValidInt("How many to buy? ");
            if (quantity <= 0) break;
            int price = (choice == 0) ? POTION_PRICE : (choice == 1) ? ELIXIR_PRICE : VITALITY_VIAL_PRICE;
            
            if (quantity * price <= player->gold) {
                player->gold -= quantity * price;
                if (choice == 0) player->potions += quantity;
                else if (choice == 1) player->elixirs += quantity;
                else if (choice == 2) { // New Permanent Upgrade Feature
                    player->vitality_vials += quantity;
                    player->maxHp += VIAL_HP_BOOST * quantity;
                    player->hp = player->maxHp; // Full heal on max HP boost
                    printf(GREEN "PERMANENT UPGRADE! Max HP is now %d.\n" RESET, player->maxHp);
                }
                printf(GREEN "Transaction successful.\n" RESET);
            } else {
                printf(RED "Not enough gold to complete transaction.\n" RESET);
            }
        } else if (choice == 3 || choice == 4) { // SELL LOGIC
            int *item_count = (choice == 3) ? &player->potions : &player->elixirs;
            int sell_price = (choice == 3) ? POTION_SELL_PRICE : ELIXIR_SELL_PRICE;

            quantity = getValidInt("How many to sell? ");
            if (quantity <= 0) break;

            if (quantity <= *item_count) {
                *item_count -= quantity;
                player->gold += quantity * sell_price;
                printf(GREEN "Sold %d items for %d gold.\n" RESET, quantity, quantity * sell_price);
            } else {
                printf(RED "Not enough items to sell.\n" RESET);
            }
        }
        wait_for_enter();
    }
}

// 2. Business Module: The Inn
void inn(Player *player) {
    clearScreen();
    displayStats(player);
    printf("\n" BLUE "--- THE OLD INN ---\n" RESET);
    printf("Resting costs %d Gold and restores full HP.\n", INN_REST_COST);
    printf("========================================\n");
    
    const char *inn_menu[] = {"Rest", "Back"};
    flush_input();
    int choice = menu_select("Choose Action", inn_menu, 2, player);
    
    if (choice == 0) {
        if (player->hp == player->maxHp) {
            printf(YELLOW "You are already at full HP!\n" RESET);
        } else if (player->gold >= INN_REST_COST) {
            player->gold -= INN_REST_COST;
            player->hp = player->maxHp;
            printf(GREEN "You slept soundly. HP restored to %d!\n" RESET, player->maxHp);
        } else {
            printf(RED "Not enough gold to pay the innkeeper.\n" RESET);
        }
    }
    wait_for_enter();
}

// Item Use Menu
void useItem(Player *player) {
    while (1) {
        clearScreen();
        displayStats(player);
        printf("\n" MAGENTA "--- USE ITEM ---\n" RESET);
        
        const char *item_menu[] = {"Use Potion (50 HP)", "Use Elixir (Full HP)", "Back"};
        flush_input();
        int choice = menu_select("Select Item", item_menu, 3, player);
        
        if (choice == 2) return;

        int used = 0;
        if (choice == 0) { // Use Potion
            if (player->potions > 0 && player->hp < player->maxHp) {
                player->potions--;
                player->hp += POTION_HEAL;
                if (player->hp > player->maxHp) player->hp = player->maxHp;
                printf(GREEN "Used Potion. HP restored to %d.\n" RESET, player->hp);
                used = 1;
            } else {
                printf(YELLOW "Cannot use: " RESET);
                if (player->potions == 0) printf(RED "No potions." RESET);
                else printf(YELLOW "Already full HP." RESET);
            }
        } else if (choice == 1) { // Use Elixir
            if (player->elixirs > 0 && player->hp < player->maxHp) {
                player->elixirs--;
                player->hp = player->maxHp;
                printf(GREEN "Used Elixir. HP fully restored to %d.\n" RESET, player->maxHp);
                used = 1;
            } else {
                printf(YELLOW "Cannot use: " RESET);
                if (player->elixirs == 0) printf(RED "No elixirs." RESET);
                else printf(YELLOW "Already full HP." RESET);
            }
        }
        if (used) {
            wait_for_enter();
            break;
        }
        wait_for_enter();
    }
}

// --- LEVELING SYSTEM (Mandatory Requirement) ---

void levelUp(Player *player) {
    int threshold = player->level * EXP_THRESHOLD_BASE;
    // Fix: Ensures EXP rollover is correct even with large gains
    while (player->exp >= threshold) { 
        player->exp -= threshold; 
        player->level++; 
        player->maxHp += 20; 
        player->hp = player->maxHp; 
        threshold = player->level * EXP_THRESHOLD_BASE; // New threshold
        printf(YELLOW "\n*** LEVEL UP! You are now Level %d! ***\n" RESET, player->level);
        printf(GREEN "Max HP increased by 20 and fully restored.\n" RESET);
    }
}

// --- DUNGEON MODULE (Math & Logic Puzzles) ---

void dungeon(Player *player) {
    int level = 1;
    // Scaling Damage & Rewards (Feature/Balance)
    int damage_penalty = BASE_DUNGEON_DAMAGE + (player->level * 2); 
    float gold_reward = 10.0 + (player->level * 5);
    int exp_reward = 20 + (player->level * 10);

    while (level <= 5) {
        clearScreen();
        displayStats(player);
        printf(RED "\n--- DUNGEON LEVEL %d ---\n" RESET, level);
        printf("HP Penalty for failure: %d HP\n", damage_penalty);
        printf("Reward for success: %.0f Gold & %d EXP\n\n", gold_reward, exp_reward);
        
        int success = 0;
        switch (level) {
            case 1: success = puzzleLevel1(); break;
            case 2: success = puzzleLevel2(); break;
            case 3: success = puzzleLevel3(); break;
            case 4: success = puzzleLevel4(); break;
            case 5: success = puzzleLevel5(); break;
        }

        if (success) {
            printf(GREEN "Correct! Puzzle solved.\n" RESET);
            player->gold += (int)gold_reward; // Cast to int for gold
            player->exp += exp_reward;
            levelUp(player);
            level++;
            wait_for_enter();
        } else {
            printf(RED "WRONG! You take %d damage.\n" RESET, damage_penalty);
            player->hp -= damage_penalty;
            if (player->hp <= 0) {
                printf(RED "\nYOU HAVE FAINTED! The trial ends.\n" RESET);
                printf("You revive back in town with a reduced penalty.\n");
                player->gold = player->gold * 9 / 10; // 10% Gold Penalty
                player->hp = player->maxHp / 5; 
                if (player->hp == 0) player->hp = 1;
                wait_for_enter();
                return;
            }
            wait_for_enter();
        }
    }
    printf(GREEN "\nDUNGEON CLEARED! You are victorious.\n" RESET);
    wait_for_enter();
}


// Puzzle 1: Geometry Gate (Area & Perimeter)
int puzzleLevel1() {
    int l = rand() % 15 + 3;
    int w = rand() % 15 + 3;
    int ansArea, ansPerim;

    printf("GEOMETRY GATE: A rectangle has Length %d and Width %d.\n", l, w);
    printf("Formula: Area = L * W | Perimeter = 2 * (L + W)\n");
    
    ansArea = getValidInt("1. What is the Area? ");
    ansPerim = getValidInt("2. What is the Perimeter? ");
    
    return (ansArea == (l * w) && ansPerim == 2 * (l + w));
}

// Puzzle 2: Physics Archer (Time = Dist/Vel)
int puzzleLevel2() {
    int dist = (rand() % 10 + 5) * 10; 
    int vel = rand() % 5 + 5;
    float ansTime;
    float correctTime = (float)dist / vel;

    printf("SKELETON ARCHER: Target is %dm away. Arrow speed is %dm/s.\n", dist, vel);
    printf("Formula: Time = Distance / Velocity\n");
    
    ansTime = getValidFloat("How many seconds to hit (to 2 decimal places)? ");
    
    return (fabs(ansTime - correctTime) < 0.01); 
}

// Puzzle 3: Bridge of Pythagoras (Hypotenuse)
int puzzleLevel3() {
    int a = rand() % 10 + 3;
    int b = rand() % 10 + 3;
    float ansHyp;
    float correctHyp = sqrt((a*a) + (b*b));

    printf("BROKEN BRIDGE: Right triangle with legs a=%d, b=%d.\n", a, b);
    printf("Formula: c = sqrt(a^2 + b^2)\n");
    
    ansHyp = getValidFloat("Calculate hypotenuse c (to 2 decimal places): ");
    
    return (fabs(ansHyp - correctHyp) < 0.01);
}

// Puzzle 4: Heavy Boulder (Accel = Force/Mass)
int puzzleLevel4() {
    int force = (rand() % 50 + 10) * 10;
    int mass = rand() % 10 + 5;
    float ansAcc;
    float correctAcc = (float)force / mass;

    printf("GOLEM BOULDER: Mass is %dkg. Force applied is %dN.\n", mass, force);
    printf("Formula: Acceleration = Force / Mass\n");
    
    ansAcc = getValidFloat("Calculate Acceleration (m/s^2, to 2 decimal places): ");
    
    return (fabs(ansAcc - correctAcc) < 0.01);
}

// Puzzle 5: The Boss (Logic) - Array Sorting (Mandatory Array Use)
int puzzleLevel5() {
    // 1. Array Usage: Declaring and initializing the required array
    int numbers[5];
    int user_sorted[5];
    int correct = 1;

    printf("LICH OF LOGIC: The final trial is the **Rune Sort**.\n");
    printf("You must enter the following 5 magical runes in ASCENDING (lowest to highest) order.\n\n");
    
    printf("Runes to sort: [ ");
    for (int i = 0; i < 5; i++) {
        numbers[i] = rand() % 99 + 1;
        printf(YELLOW "%d" RESET, numbers[i]);
        if (i < 4) printf(", ");
    }
    printf(" ]\n\n");

    // 2. Get user input
    for (int i = 0; i < 5; i++) {
        char prompt[70];
        sprintf(prompt, "Enter number %d of 5 in the sorted sequence: ", i + 1);
        user_sorted[i] = getValidInt(prompt);
    }

    // 3. Sort the original array (Simple Bubble Sort for verification)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++) {
            if (numbers[j] > numbers[j+1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j+1];
                numbers[j+1] = temp;
            }
        }
    }

    // 4. Compare user input with the correctly sorted array
    for (int i = 0; i < 5; i++) {
        if (user_sorted[i] != numbers[i]) {
            correct = 0;
            break;
        }
    }
    
    if (!correct) {
        printf("\nIncorrect! The correct sorted order was: [ ");
        for (int i = 0; i < 5; i++) {
            printf(GREEN "%d" RESET, numbers[i]);
            if (i < 4) printf(", ");
        }
        printf(" ]\n");
    }

    return correct;
}