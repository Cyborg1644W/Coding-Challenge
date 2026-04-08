#include <iostream>
#include <string>

using std::cin;
using std::endl;
using std::cout;
using std::string;

#ifdef _WIN32 
    void clear_screen() {
        system("cls");
        cout << "\n\n";
    }
    #else
    void clear_screen() {
        system("clear");
        cout << "\n\n";
    }    
#endif

#define RED     "\033[0;31m"
#define B_GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[0;34m"
#define CYAN  "\033[0;36m"
#define RESET   "\033[0m"

int displayMenu();
void wait_for_enter();
void delete_previous_output();
int getPositiveInt(string prompt);
string getStringNoNumbers(string prompt);
void getSale(string names[], int sales[], int& counter);
void displaySales(string names[], int sales[], int& counter);


int main() {

    string itemNames[100];
    int itemSales[100];
    int counter = 0;

    bool isRunning = true;

    while(isRunning){
        switch(displayMenu()) {
            case 1:
                getSale(itemNames, itemSales, counter);
                break;
            case 2:
                displaySales(itemNames, itemSales, counter);
                break;
            case 3:
                break;
            case 4:
                break; 
                break;
            default:
                break;
        }
    }
    return 0;
}

int displayMenu() {
    clear_screen();
    int choice;
    cout << YELLOW"[1] " CYAN"Enter Sales\n" RESET;
    cout << YELLOW"[2] " CYAN"Display Sales\n" RESET;
    cout << YELLOW"[3] " CYAN"Sales Ranking\n" RESET;
    cout << YELLOW"[4] " CYAN"Bar Graph\n" RESET;
    cout << YELLOW"[5] " CYAN"Exit\n" RESET;

    do {
        cout << BLUE "\n";
        choice = getPositiveInt("--> ");
    } while(choice < 1 || choice > 5);

    return choice;
}

void wait_for_enter() {
    cout << YELLOW "Press Enter to Continue..";
    getchar();
    delete_previous_output();
}

void delete_previous_output() {
    cout << "\033[F\033[K";
    cout << "\033[F\033[K";
    cout << "\033[F\033[K";
}

int getPositiveInt(string prompt) {
    int x;

    while (true) {
        cout << BLUE + prompt;
        cout << B_GREEN;
        cin >> x;
        cout << RESET;
        
        if (cin.fail() || x <= 0) {
            cin.clear();
            cin.ignore(1000, '\n'); // simple cleanup
            cout << RED "Positive Integer Only\n" RESET;
            wait_for_enter();
            
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

string getStringNoNumbers(string prompt) {
    string input;

    while (true) {
        cout << BLUE + prompt;
        cout << B_GREEN;
        getline(cin, input);

        if (input == "") {
            cout << RED "Input cannot be empty.\n" RESET;
            wait_for_enter();
            continue;
        }

        bool valid = true;
        for (char c : input) {
            if (isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (valid)
            return input;
        cout << RED "Letters only. Try again.\n" RESET;
        wait_for_enter();
    }
}

void getSale(string names[], int sales[], int& counter) {
    clear_screen();
    names[counter] = getStringNoNumbers("Enter The Item Name : ");
    sales[counter] = getPositiveInt("Enter The Item Sales : ");
    counter++;
    cout << CYAN "Item Successfully Added\n";
    wait_for_enter();

}

void displaySales(string names[], int sales[], int& counter) {
    clear_screen();
    for(int i = 0; i < counter; i++) {

    }
}



