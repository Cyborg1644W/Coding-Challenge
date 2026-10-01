#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <conio.h>

using namespace std;

const string ACCOUNTS_FILE = "newAccounts.csv";
const double MIN_INITIAL_DEPOSIT = 5000.00;

struct AccountRecord {
    string accNo;
    string accName;
    string birthday;
    string contact;
    double balance;
    string PINcode;   // stored ENCRYPTED, never plaintext
    bool active;      // false while on hold
};

struct Node {
    AccountRecord data;
    Node *next;
};

class ATMList {
private:
    Node* head;
public:
    ATMList() { head = NULL; }
    ~ATMList() {
        while (head != NULL) {
            Node *p = head;
            head = head->next;
            delete p;
        }
    }
    void insertCard(const AccountRecord& account);
    Node* locate(string accNo);
    Node* getHead() { return head; }
    bool removeAcc(const string &accNo);
};

void ATMList::insertCard(const AccountRecord& account) {
    Node* newNode = new Node();
    newNode->data = account;
    newNode->next = head;
    head = newNode;
}

bool ATMList::removeAcc(const string &accNo) {
    Node *current = head, *prev = NULL;
    while (current != NULL) {
        if (current->data.accNo == accNo) {
            if (prev == NULL) head = current->next;
            else prev->next = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

Node* ATMList::locate(string accNo) {
    Node* current = head;
    while (current != NULL) {
        if (current->data.accNo == accNo) return current;
        current = current->next;
    }
    return NULL;
}

// ---------------------------------------------------------------------
// Validation / security helpers
// ---------------------------------------------------------------------

bool isAllDigits(const string &s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); i++)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

// Password astiris
string printMaskedInput() {
    string pin = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pin.empty()) {
            pin.erase(pin.length() - 1);
            cout << "\b \b";
        } else if (ch >= '0' && ch <= '9') {
            pin += ch;
            cout << '*';
        }
    }
    cout << endl;
    return pin;
}

// Simple reversible digit-shift "encryption" so raw PINs are never stored as-is.
string encryptPin(const string &pin) {
    string enc = pin;
    for (size_t i = 0; i < enc.size(); i++) {
        int d = enc[i] - '0';
        d = (d + 3) % 10;
        enc[i] = (char)('0' + d);
    }
    return enc;
}

string decryptPin(const string &enc) {
    string pin = enc;
    for (size_t i = 0; i < pin.size(); i++) {
        int d = pin[i] - '0';
        d = (d + 10 - 3) % 10;
        pin[i] = (char)('0' + d);
    }
    return pin;
}

// ---------------------------------------------------------------------
// UI Helper Functions
// ---------------------------------------------------------------------

void typeOut(const string &text, int delayMs = 40) {
    for (size_t i = 0; i < text.size(); i++) {
        cout << text[i] << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

void loadingScreen() {
        system("cls");

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\t\t\t\t\t";
        typeOut("        Your card chip is being processed.\n", 45);
        cout << "\t\t\t                                                                            \n";
        cout << "\t\t\t                   ____________________________________________             \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |  BANKERBONIA                               |            \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |  :::::    XX XXX                           |            \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |  VALID                                     |            \n";
        cout << "\t\t\t                  |  THRU XX/XX                                |            \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |                                            |            \n";
        cout << "\t\t\t                  |  CARDHOLDER NAME                           |            \n";
        cout << "\t\t\t                  |____________________________________________|            \n";
        cout << "\t\t\t                                                                            \n";
        cout << "\t\t\t                                                                            \n";
        cout << "\t\t\t                                                                            \n";


        cout << "\e[?25l"; // Hide cursor

        typeOut("\t\t\t\t\t\t\t   Please wait", 45);


    for (int cycle = 0; cycle < 4; cycle++) {
        for (int dots = 0; dots <= 3; dots++) {
            cout << "\r\t\t\t\t\t\t\t   Please wait" << string(dots, '.') << string(3 - dots, ' ') << flush;
            this_thread::sleep_for(chrono::milliseconds(300));
        }
    }
    cout << "\r\t\t\t\t\t\t\tCard chip verified!                  \n\n";
    this_thread::sleep_for(chrono::milliseconds(600));
    cout << "\e[?25h"; // Show cursor
    system("cls");
}

// ---------------------------------------------------------------------
// Registration
// ---------------------------------------------------------------------

class Registration {
public:
    void regAcc(ATMList& atm) {
        system("cls");
        AccountRecord account;
        account.active = true;
        string pin, pinConfirm;

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

        // Account number: must be exactly 5 digits AND unique
        while (true) {
            cout << "\t\t\t\t        [1] Enter Account Number (5 digits): ";
            cin >> account.accNo;
            cin.ignore(10000, '\n');

            if (account.accNo.size() != 5 || !isAllDigits(account.accNo)) {
                cout << "\t\t\t\t            Invalid. Account number must be exactly 5 digits.\n";
                continue;
            }
            if (atm.locate(account.accNo) != NULL) {
                cout << "\t\t\t\t            That account number is already taken.\n";
                continue;
            }
            break;
        }

        cout << "\t\t\t\t        [2] Enter Account Name: ";
        getline(cin, account.accName);

        string birthday;
        int valid;

        while (true) {
            valid = 1;

            cout << "\t\t\t\t        [3] Input Birthday (YYYY/MM/DD): ";
            getline(cin, account.birthday);

            if (account.birthday.length() != 10) {
                valid = 0;
            } else {
                if(account.birthday[4] != '/' || account.birthday[7] != '/') {
                    valid = 0;
                }
                else {
                    for (int i = 0; i < 10; i++) {
                        if(i != 4 && i != 7) {
                            if(account.birthday[i] < '0' || account.birthday[i] > '9') {
                                valid = 0;
                            }
                        }
                    }
                }
            }

            if (valid == 1) {
                break;
            } else {
                cout << "\t\t\t\t            Invalid birthday! Use YYYY/MM/DD\n";
            }
        }


        bool validContact = false;
        while (!validContact) {
            cout << "\t\t\t\t        [4] Enter Contact Number: ";
            getline(cin, account.contact);

            validContact = (account.contact.length() == 11) && isAllDigits(account.contact);

            if (!validContact) {
                cout << "\t\t\t\t            Invalid input. Please enter exactly 11 numbers.\n";
            }
        }

        cout << "\t\t\t\t        [5] Enter Initial Deposit (Min. Php "
             << fixed << setprecision(2) << MIN_INITIAL_DEPOSIT << "): ";
        while (true) {
            if (!(cin >> account.balance)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\t\t\t\t            Invalid Input. Enter Initial Deposit: ";
                continue;
            }
            if (account.balance < MIN_INITIAL_DEPOSIT) {
                cout << "\t\t\t\t            Deposit must be at least Php "
                     << fixed << setprecision(2) << MIN_INITIAL_DEPOSIT
                     << "\n\t\t\t\t\t[5]Enter Initial Deposit: ";
                continue;
            }
            break;
        }

        while (true) {
            cout << "\t\t\t\t        [6] Enter PIN Code (4 or 6 digits): ";
            cin >> pin;

             if (!((pin.size() == 4 || pin.size() == 6) && isAllDigits(pin))) {
                cout << "\t\t\t\t        Invalid. PIN must be exactly 4 or 6 digits and contain only numbers.\n\n";
                continue;
            }
            cout << "\t\t\t\t        [7] Confirm PIN Code: ";
            cin >> pinConfirm;

            if (pin != pinConfirm) {
                cout << "\t\t\t\t        PINs do not match. Try again.\n\n";
                continue;
            }
            break;
        }

        account.PINcode = encryptPin(pin);   // never store the raw PIN

        atm.insertCard(account);
        cout << "\n\t\t\t\t\t      Account " << account.accNo << " successfully registered!\n";
        this_thread::sleep_for(chrono::seconds(2));
    }
};
void saveAccountsFile(ATMList& atm);

// ---------------------------------------------------------------------
// Transactions
// ---------------------------------------------------------------------

class Transaction {
public:
    void balInquiry(AccountRecord& account) {
        system("cls");
        int choice;

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                         | 1.) SAVINGS          |       |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                         | 2.) CURRENT          |       |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   | -----------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|        Press The Right Key:                                            |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

        cout << "\033[3A\033[59C" << flush;
        while (true) {
            if (cin >> choice) {
                if (choice == 1) {
                    inqBalSav(account);
                    break;
                } else if (choice == 2) {
                    inqBalCur(account);
                    break;
                } else {
                    cout << "\n\n\t\t\t\t Invalid Choice. Select (1-2): ";
                }
            } else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\n\n\t\t\t\t Invalid Input. Select (1-2): ";
            }
        }
    }

    void inqBalCur(AccountRecord& account) {
        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  YOUR BALANCE IS:                                       |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   ACCOUNT NO.:            | " << account.accNo << "                |      |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   CURRENT:                | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[3A" << flush;
    }
        void inqBalSav(AccountRecord& account) {
        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  YOUR BALANCE IS:                                       |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   ACCOUNT NO.:            | " << account.accNo << "                |      |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   SAVINGS:                | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[3A" << flush;
    }

    void fastCash(AccountRecord& account) {
        system("cls");
        int choice;
        double amount = 0;

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |       | 1.) 500              |          |  5.) 5000            |       |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |       | 2.) 2000             |          |  6.) 6000            |       |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |       | 3.) 3000             |          |  7.) 8000            |       |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |       | 4.) 4000             |          |  8.) 10000           |       |   | \n";
        cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   | -----------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|        Press The Right Key:                                            |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

        cout << "\033[3A\033[59C" << flush;

        while (!(cin >> choice) || choice < 1 || choice > 8) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\t\t\t\t Invalid choice. Select (1-8): ";
        }

        switch (choice) {
            case 1: amount = 500; break;
            case 2: amount = 2000; break;
            case 3: amount = 3000; break;
            case 4: amount = 4000; break;
            case 5: amount = 5000; break;
            case 6: amount = 6000; break;
            case 7: amount = 8000; break;
            case 8: amount = 10000; break;
        }

        if (amount > account.balance) {
            cout << "\n\n\t\t\t\t\t\t\tInsufficient Funds!\n";
        } else {
            account.balance -= amount;
            system("cls");

            cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
            cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
            cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
            cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
            cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
            cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
            cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
            cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  PLEASE TAKE YOUR CASH, YOUR NEW BALANCE IS:            |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   ACCOUNT NO.:            | " << account.accNo << "                |      |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   NEW BALANCE:            | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |___|                                                                        |___| \n";
            cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

            cout << "\033[3A" << flush;
        }

    }

    void withdraw(AccountRecord& account) {
        system("cls");
        int typeChoice;
        double amount;
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                         | 1.) SAVINGS          |       |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                         | 2.) CURRENT          |       |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                         | 3.) FAST CASH        |       |   | \n";
        cout << "\t\t\t |   |                                         |----------------------|       |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   | -----------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|        Choose Transaction:                                             |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

       cout << "\033[3A\033[58C" << flush;

        while (!(cin >> typeChoice) || typeChoice < 1 || typeChoice > 3) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\t\t\t       Invalid choice. Select (1-3): ";
        }

        if (typeChoice == 3) {
            fastCash(account);
            return;
        }

        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 Available Balance:                      |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |          PLEASE ENTER DESIRED AMOUNT AND CONFIRM:       |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        while (!(cin >> amount) || amount < 100) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\n\n\n\n\t\t\t\t\tInvalid (Minimum Withdrawal is Php 100). \n\t\t\t\t\tEnter Amount: ";
        }

        if (amount > account.balance) {
            cout << "\n\n\n\n\n\t\t\t\t\t\t\t Insufficient Funds!\n";
        } else {
            account.balance -= amount;
            cout << "\t\t\t       Please take your cash.\n";

        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  PLEASE TAKE YOUR CASH, YOUR NEW BALANCE IS:                                       |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   ACCOUNT NO:             | " << account.accNo << "                |      |      |   | \n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   NEW BALANCE:            | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
        cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        }
    }

    void deposit(AccountRecord& account) {
        system("cls");
        double amount;

        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 Available Balance:                      |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |          PLEASE ENTER DESIRED AMOUNT AND CONFIRM:       |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        while (!(cin >> amount) || amount <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\n\n\n\n\t\t\t\t Invalid. Enter Amount: ";
        }

        account.balance += amount;
            system("cls");

            cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
            cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
            cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
            cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
            cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
            cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
            cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
            cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  DEPOSIT SUCCESSFUL! YOUR NEW BALANCE IS:               |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   ACCOUNT NO.:            | " << account.accNo << "                |      |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   NEW BALANCE:            | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |___|                                                                        |___| \n";
            cout << "\t\t\t( ___ )                                                                      ( ___ )\n";


    }

    void fundTransfer(AccountRecord& account, ATMList& atm) {
        string destAcc;

        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |            PLEASE ENTER DESTINATION ACCOUNT NO.:        |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        cin >> destAcc;
        cin.ignore(10000, '\n');

        if (destAcc == account.accNo) {
            cout << "\n\n\n\n\n\t\t\t\t You cannot transfer to your own account.\n";
            return;
        }

        Node* dest = atm.locate(destAcc);
        if (dest == NULL) {
            cout << "\n\n\n\n\n\t\t\t\t Destination account not found.\n";
            return;
        }

        double amount;
        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 Available Balance:                      |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |          PLEASE ENTER DESIRED AMOUNT AND CONFIRM:       |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        while (!(cin >> amount) || amount <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n\n\n\n\n\t\t\t\t     Invalid. Enter Amount: ";
        }

        if (amount > account.balance) {
            cout << "\n\n\n\n\n\t\t\t\t     Insufficient Funds!\n";
            return;
        }

        account.balance -= amount;
        dest->data.balance += amount;
        system("cls");

        system("cls");

            cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
            cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
            cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
            cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
            cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
            cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
            cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
            cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |  TRANSFERRED SUCCESSSFULLY, YOUR NEW BALANCE IS:        |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   ACCOUNT NO.:            | " << account.accNo << "                |      |      |   | \n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   NEW BALANCE:            | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |   AVAILABLE:              | Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|      |      |   |\n";
            cout << "\t\t\t |   |       |                           |----------------------|      |      |   | \n";
            cout << "\t\t\t |   |       |                                                         |      |   | \n";
            cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |___|                                                                        |___| \n";
            cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

            cout << "\033[3A" << flush;

    }

    void changePINCode(AccountRecord& account) {
        system("cls");
        string oldPin, newPin, confirmPin;
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |             PLEASE ENTER PIN CODE TO CONTINUE:          |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        oldPin = printMaskedInput();//pass astiris

        if (encryptPin(oldPin) != account.PINcode) {
            cout << "\n\n\n\n\n\t\t\t\t Incorrect PIN.\n";
            return;
        }

        while (true) {

        system("cls");
        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 PLEASE ENTER NEW PIN:                   |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 CONFIRM NEW PIN:                        |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[14A\033[57C" << flush;
        newPin = printMaskedInput();
        cout << "\033[4B\033[57C" << flush;
        confirmPin = printMaskedInput();


        cout << "\n\n\n\n";

            if (newPin != confirmPin) {
                cout << "\n\t\t\t\t PINs do not match. Try again.\n\n\n";
                system("pause");
                continue;
            }
            if (!((newPin.size() == 4 || newPin.size() == 6) && isAllDigits(newPin))) {
                cout << "\n\t\t\t\t PIN must be exactly 4 or 6 digits. Try again.\n\n\n";
                system("pause");
                continue;
            }
            break;
        }

        account.PINcode = encryptPin(newPin);
        cout << "\n\t\t\t\t PIN Successfully Changed!\n";
    }

    // Returns true if the session should end (account is now on hold)
    bool holdAccount(AccountRecord& account) {
        system("cls");

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                Place this account on HOLD?              |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |           You will not be able to log in again,         |      |   | \n";
        cout << "\t\t\t |   |       |              until the bank lifts the hold.             |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |(Y/N):                |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

        cout << "\033[10A\033[63C" << flush;
        string confirm;
        cin >> confirm;

        if (!confirm.empty() && (confirm[0] == 'Y' || confirm[0] == 'y')) {
            account.active = false;
            cout << "\n\n\n\n\n\t\t\t\t\t\t Your Account has been Placed on Hold.\n\n\n";
            system("pause");
            return true;
        }
        cout << "\n\n\n\n\n\t\t\t\t\t\t Cancelled Account Hold Process...\n";

        return false;
    }
// Returns true if the session should end (account is now terminated)
    bool terminateAccount(AccountRecord& account, ATMList& atm) {
        system("cls");

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 Terminate this Account?                 |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |           You will not be able to log in again,         |      |   | \n";
        cout << "\t\t\t |   |       |                  This CANNOT be undone.                 |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |(Y/N):                |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

        cout << "\033[10A\033[63C" << flush;
        string confirm;
        cin >> confirm;

        if (confirm.empty() || (confirm[0] != 'Y' && confirm[0] != 'y')) {
            cout << "\n\n\n\n\n\t\t\t\t\t       Account Termination Process Cancelled...\n\n\n";
            system("pause");
            return false;
        }

        string pin;
        system("cls");

        cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
        cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
        cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
        cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
        cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
        cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
        cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
        cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |            PLEASE ENTER PIN CODE TO CONFIRM:            |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
        cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
        cout << "\t\t\t |   |       |                                                         |      |   | \n";
        cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
        cout << "\t\t\t |   |                                                                        |   | \n";
        cout << "\t\t\t |___|                                                                        |___| \n";
        cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

        cout << "\033[9A\033[57C" << flush;

        pin = printMaskedInput();
        if (encryptPin(pin) != account.PINcode) {
            cout << "\n\n\n\n\n\t\t\t\t\t       Incorrect PIN. Termination Process Cancelled...\n\n\n";
            system("pause");
            return false;
        }



        while (account.balance > 0) {
            system("cls");
            cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
            cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
            cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
            cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
            cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
            cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
            cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
            cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |          _________________________________________________________     |   | \n";
            cout << "\t\t\t |   |         |                                                         |    |   | \n";
            cout << "\t\t\t |   |         |  WARNING: Your Account Has A Balance Of Php " << left << setw(17) << fixed << setprecision(2) << account.balance <<"|    |   | \n";
            cout << "\t\t\t |   |         |      You Must Empty Your Account Before Termination.    |    |   | \n";
            cout << "\t\t\t |   |         |                                                         |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |               | 1.) Withdraw All Funds |                |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |                                                         |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |               | 2.) Transfer All Funds |                |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |                                                         |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |               | 3.) Cancel Termination |                |    |   | \n";
            cout << "\t\t\t |   |         |               |------------------------|                |    |   | \n";
            cout << "\t\t\t |   |         |_________________________________________________________|    |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |         Press The Right Key: 			                      |   | \n";
            cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

            cout << "\033[3A\033[60C" << flush;
            int choice;

            while (!(cin >> choice) || choice < 1 || choice > 3) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\t\t\t\t       Invalid Input (Please choose 1-3): ";
            }

            if (choice == 1) {
                cout << "\n\t\t\t\t\t\t\tDispensing Php " << fixed << setprecision(2) << account.balance << "...\n";
                account.balance = 0;
                cout << "\t\t\t\t\t\t\tPlease Take Your Cash.\n";
                system("pause");
            } else if (choice == 2) {
                string destAcc;

                system("cls");
                cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
                cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
                cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
                cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
                cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
                cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
                cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
                cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |  BANKERBONIA BANKS                                      |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |               Enter Destination Account No.:            |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |___|                                                                        |___| \n";
                cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

                cout << "\033[9A\033[57C" << flush;
                cin >> destAcc;
                cin.ignore(10000, '\n');

                if (destAcc == account.accNo) {
                    cout << "\n\n\n\n\n\t\t\t\t\t\t   Cannot Transfer To Your Own Account.\n\n\n";
                    system("pause");
                    continue;
                }

                Node* dest = atm.locate(destAcc);
                if (dest == NULL) {
                    cout << "\n\n\n\n\n\t\t\t\t\t\t     Destination Account Not Found.\n\n\n";
                    system("pause");
                    continue;
                }

                dest->data.balance += account.balance;
                cout << "\n\n\n\n\n\t\t\t\t  Php " << fixed << setprecision(2) << account.balance
                     << " Successfully Transferred To Account " << destAcc << ".\n";
                account.balance = 0;
            } else {
                cout << "\n\t\t\t\t\t       Account Termination Process Cancelled...\n\n\n";
                system("pause");
                return false;
            }
        }

        // Account is now empty and PIN is verified. Safe to terminate.
        string accNo = account.accNo;
        atm.removeAcc(accNo);
        cout << "\n\t\t\t\t\t\t    Account Terminated Successfully.\n\n\n";
        system("pause");
        return true;
    }
    void getYourCard() {
    system("cls");
                cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
                cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
                cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
                cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
                cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
                cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
                cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
                cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |                         TRANSACTION COMPLETED.                         |   | \n";
                cout << "\t\t\t |   |                 _______________________________________                |   | \n";
                cout << "\t\t\t |   |                 _______________________________________                |   | \n";
                cout << "\t\t\t |   |                    ||                             ||                   |   | \n";
                cout << "\t\t\t |   |                    ||   _______________________   ||                   |   | \n";
                cout << "\t\t\t |   |                    ||    ||                 ||    ||                   |   | \n";
                cout << "\t\t\t |   |                    ||    ||         #####   ||    ||                   |   | \n";
                cout << "\t\t\t |   |                    ||_____          #####    _____||                   |   | \n";
                cout << "\t\t\t |   |                          ||         #####  ||                          |   | \n";
                cout << "\t\t\t |   |                          ||                ||                          |   | \n";
                cout << "\t\t\t |   |                          ||       ||       ||                          |   | \n";
                cout << "\t\t\t |   |                          ||     \\ || /     ||                          |   | \n";
                cout << "\t\t\t |   |                          ||      \\||/      ||                          |   | \n";
                cout << "\t\t\t |   |                          ||       \\/       ||                          |   | \n";
                cout << "\t\t\t |   |                          ||                ||                          |   | \n";
                cout << "\t\t\t |   |                           |________________|                           |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |                          PLEASE GET YOUR CARD.                         |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |___|                                                                        |___| \n";
                cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";
}


    void TransactMenu(AccountRecord& account, ATMList& atm) {
        int choice;
        while (true) {
            system("cls");
            cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
            cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
            cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
            cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
            cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
            cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
            cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
            cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |       | 1.) Balance Inquiry  |          | 5.) Change PIN Code  |       |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |       | 2.) Withdraw         |          | 6.) Account Hold     |       |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |       | 3.) Deposit          |          | 7.) Terminate ACC    |       |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |       | 4.) Fund Transfer    |          | 8.) Exit             |       |   | \n";
            cout << "\t\t\t |   |       |----------------------|          |----------------------|       |   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |   | -----------------------------------------------------------------------|   | \n";
            cout << "\t\t\t |   |                                                                        |   | \n";
            cout << "\t\t\t |___|        Choose Transaction:                                             |___| \n";
            cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

            cout << "\033[3A\033[58C" << flush;

            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
            }

            switch (choice) {
                case 1: balInquiry(account); break;
                case 2: withdraw(account); break;
                case 3: deposit(account); break;
                case 4: fundTransfer(account, atm); break;
                case 5: changePINCode(account); break;
                case 6: if (holdAccount(account)) {
                        saveAccountsFile(atm); // Save the hold status
                        getYourCard();         // Display the card exit screen
                        exit(0);                // Exit the session
                    }
                    break;
                case 7: if (terminateAccount(account, atm)) {
                        saveAccountsFile(atm); // Crucial: Save the account removal to the CSV
                        getYourCard();         // Display the card exit screen
                        exit(0);                // Exit the session
                    }
                    break;
                case 8: saveAccountsFile(atm);
                        getYourCard();
                        return;
                default: cout << "Invalid Choice.\n"; system("pause");
            }
            char anotherTransact;
            while (true) {
                cout << "\n\t\t\t\t Would You Like Another Transaction? (Y/N) ";
                cin >> anotherTransact;

                cin.clear();
                cin.ignore(1000,'\n');

                if (anotherTransact == 'y' || anotherTransact == 'Y') {
                    break;
                } else if (anotherTransact == 'n' || anotherTransact == 'N') {
                    saveAccountsFile(atm);
                    getYourCard();
                    exit(0);
                    return;
                } else {
                    cout << "\n\t\t\t\t Invalid Input. Please Enter 'Y/y' Or 'N/n'.\n";
                }
            }
        }
    }
};

// ---------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------

void saveAccountsFile(ATMList& atm) {
    ofstream file(ACCOUNTS_FILE.c_str());
    if (!file.is_open()) {
        cerr << "Error: Could not save accounts file.\n";
        return;
    }
    Node* current = atm.getHead();
    while (current != NULL) {
        file << current->data.accNo << ","
             << current->data.accName << ","
             << current->data.birthday << ","
             << current->data.contact << ","
             << fixed << setprecision(2) << current->data.balance << ","
             << current->data.PINcode << ","
             << current->data.active << "\n";
        current = current->next;
    }
    file.close();
}

void loadAccountsFile(ATMList& atm) {
    ifstream file(ACCOUNTS_FILE.c_str());
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        AccountRecord acc;

        getline(ss, acc.accNo, ',');
        getline(ss, acc.accName, ',');
        getline(ss, acc.birthday, ',');
        getline(ss, acc.contact, ',');

        getline(ss, token, ',');
        acc.balance = atof(token.c_str());

        getline(ss, acc.PINcode, ',');

        if (getline(ss, token, ',')) {
            acc.active = (token != "0");
        } else {
            acc.active = true; // older files with no "active" column
        }

        atm.insertCard(acc);
    }
    file.close();
}

// ---------------------------------------------------------------------
// Outer menu
// ---------------------------------------------------------------------

int mainMenu() {
    int choice;
    system("cls");
    cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
    cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
    cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
    cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
    cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
    cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
    cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
    cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
    cout << "\t\t\t |   |                                                                        |   | \n";
    cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
    cout << "\t\t\t |   |       __________________________________________________________       |   | \n";
    cout << "\t\t\t |   |      |                                                          |      |   | \n";
    cout << "\t\t\t |   |      |   BANKERBONIA BANKS                                      |      |   | \n";
    cout << "\t\t\t |   |      |                                                          |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                 | 1.) Registration     |                 |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                                                          |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                 | 2.) Transaction      |                 |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                                                          |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                 | 3.) Exit             |                 |      |   | \n";
    cout << "\t\t\t |   |      |                 |----------------------|                 |      |   | \n";
    cout << "\t\t\t |   |      |                                                          |      |   | \n";
    cout << "\t\t\t |   |      |__________________________________________________________|      |   | \n";
    cout << "\t\t\t |   |                                                                        |   | \n";
    cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
    cout << "\t\t\t |   |                                                                        |   | \n";
    cout << "\t\t\t |   |         Press The Right Key: 			                      |   | \n";
    cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

    cout << "\033[3A\033[60C" << flush;

    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\t\t\t\t       Invalid Input (Please choose 1-3): \n\n";
        cout << "\033[2A\033[76C" << flush;

    }
    return choice;
}

int main() {
    ATMList atm;
    loadAccountsFile(atm);
    Registration reg;
    Transaction trans;

    while (true) {
        switch (mainMenu()) {
            case 1:
                reg.regAcc(atm);
                saveAccountsFile(atm);
                break;
            case 2: {
                loadingScreen();
                system("cls");
                string accNo, pin;
                cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
                cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
                cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
                cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
                cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
                cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
                cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
                cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |  BANKERVONIA BANKS                                      |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |            PLEASE ACCOUNT NUMBER AND CONFIRM:           |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |___|                                                                        |___| \n";
                cout << "\t\t\t( ___ )                                                                      ( ___ )\n";

                cout << "\033[9A\033[57C" << flush;

                cin >> accNo;
                Node* found = atm.locate(accNo);

                if (found != NULL) {
                    if (!found->data.active) {
                        cout << "\n\n\n\n\n\n\t\t\t\t\t This account is on hold. Please contact the bank.\n\n\n\n\n";
                        system("pause");
                        break;
                    }
                system("cls");
                cout << "\t\t\t                                        |\\__/,|   (`\\ \n";
                cout << "\t\t\t  ___                                 _.|o o  |_   ) )                         ___  \n";
                cout << "\t\t\t( ___ )-------------------------------(((---(((------------------------------( ___ ) \n";
                cout << "\t\t\t |   |    ____    _    _   _ _  _______ ____  ____   ___  _   _ ___    _      |   | \n";
                cout << "\t\t\t |   |   | __ )  / \\  | \\ | | |/ /_____|  _ \\| __ ) / _ \\| \\ | |_ _|  / \\     |   | \n";
                cout << "\t\t\t |   |   |  _ \\ / _ \\ |  \\| | ' /|  _| | |_) |  _ \\| | | |  \\| || |  / _ \\    |   | \n";
                cout << "\t\t\t |   |   | |_) / ___ \\| |\\  | . \\| |___|  _ <| |_) | |_| | |\\  || | / ___ \\   |   | \n";
                cout << "\t\t\t |   |   |____/_/   \\_\\_| \\_|_|\\_\\_____|_| \\_\\____/ \\___/|_| \\_|___/_/   \\_\\  |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |        _________________________________________________________       |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |  BANKERVONIA BANKS                                      |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |           PLEASE ENTER PIN CODE TO CONTINUE:            |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                 |                      |                |      |   | \n";
                cout << "\t\t\t |   |       |                 |----------------------|                |      |   | \n";
                cout << "\t\t\t |   |       |                                                         |      |   | \n";
                cout << "\t\t\t |   |       |_________________________________________________________|      |   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |   |------------------------------------------------------------------------|   | \n";
                cout << "\t\t\t |   |                                                                        |   | \n";
                cout << "\t\t\t |___|                                                                        |___| \n";
                cout << "\t\t\t( ___ )                                                                      ( ___ )\n\n";

                 cout << "\033[10A\033[57C" << flush;

                    pin = printMaskedInput();
                    if (encryptPin(pin) == found->data.PINcode) {
                        trans.TransactMenu(found->data, atm);
                        saveAccountsFile(atm);
                    } else {
                        cout << "\n\n\n\n\n\n\t\t\t\t\t\t\t     Invalid PIN.\n\n\n\n\n";
                        system("pause");
                    }
                } else {
                    cout << "\n\n\n\n\n\n\t\t\t\t\t\t\t ACCOUNT NOT FOUND.\n\n\n\n\n";
                    system("pause");
                }
                break;
            }
            case 3:

                system("cls");
                trans.getYourCard();
                exit(0);
                break;


            default:
                cout << "\t\t\t\t       Invalid Input (Please choose 1-3): \n\n";
                system("pause");
        }
    }
    return 0;
}
