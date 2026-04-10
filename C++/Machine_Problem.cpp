#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>

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
void displaySales(string names[], int sales[], int counter);
void sortSales(string names[], int sales[], int counter);
void displayGraph(string names[], int sales[], int counter);


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
                clear_screen(); 
                if (counter == 0) {
                    cout << RED "[!] ERROR: NO DATA LOGGED\n";
                    wait_for_enter();
                    break;
                }
                cout << CYAN "===========SALES===========\n" << endl;
                displaySales(itemNames, itemSales, counter);
                break;
            case 3:
                sortSales(itemNames, itemSales, counter);
                break;
            case 4:
                displayGraph(itemNames, itemSales, counter);
                break; 
            case 5:
                isRunning = false;
                clear_screen();
                cout << B_GREEN "  [!] STATUS: SYSTEM SHUTDOWN SUCCESSFUL\n";
                cout << "  [i] ACTION: You may now safely close this window.\n";
                cout << "  [i] INFO  : Thank you for using Sales Tracking System v1.0\n" RESET;
                break;
            default:
                cout << RED "Input out of range. Maximum value allowed is 5.\n";
                wait_for_enter();
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

    cout << BLUE "\n";
    choice = getPositiveInt("--> ");
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
            cout << RED "[i] ACTION: Please enter numbers only (e.g., 1500 or 99.50).\n" RESET;
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
            cout << RED "[i] ACTION: Input field cannot be empty.\n" RESET;
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
        cout << RED "[i] ALERT: Use letters only." RESET;
        wait_for_enter();
    }
}

void getSale(string names[], int sales[], int& counter) {
    while(1) {
        clear_screen();
        cout << YELLOW "TOTAL ITEM : " << counter << RESET << endl;
        string ask = getStringNoNumbers("Enter The Item Name ('Q' to quit) : ");
        if (ask == "q" || ask == "Q") {
            break;
        }
        names[counter] = ask;
        sales[counter] = getPositiveInt("Enter The Item Sales : ");
        counter++;

    }
}

void displaySales(string names[], int sales[], int counter) {
    printf("                  Quantity\n");
    for(int i = 0; i < counter; i++) {
        printf("%-15s :  %8d\n", names[i].c_str(), sales[i]);
    }
    wait_for_enter();
}

void sortSales(string names[], int sales[], int counter) {
    clear_screen();
    if (counter == 0) {
        cout << RED "[!] ERROR: NO DATA LOGGED\n";
        wait_for_enter();
    } else {
        string tempNames[100];
        int tempSales[100];
        for (int i = 0; i < counter; i++) {
            tempNames[i] = names[i];
            tempSales[i] = sales[i];
        }
        
        for (int i = 0 ; i < counter - 1; i++) {
            for(int j = 0; j < counter - i - 1; j++) {
                if (tempSales[j] < tempSales[j + 1]) {
                    int temp = tempSales[j+1];
                    tempSales[j + 1] = tempSales[j];
                    tempSales[j] = temp;
                    
                    //swap names
                    string holdName = tempNames[j + 1];
                    tempNames[j + 1] =  tempNames[j];
                    tempNames[j] = holdName;
                }
            }
        }
        cout << CYAN "=======SALES RANKING=======\n";
        displaySales(tempNames, tempSales, counter);
    }
}

void displayGraph(string names[], int sales[], int counter) {
    clear_screen();
    if (counter == 0) {
        cout << RED "[!] ERROR: NO DATA LOGGED\n";
    } else {
        for (int i = 0; i < counter; i++) {
            printf("%-15s :  ", names[i].c_str());
            for (int j = 0; j < sales[i]; j++) {
                cout << "\xDB"; //hex for solid block
            }
            cout << "\n";
        }
    }
    wait_for_enter();
}