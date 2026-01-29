//Scenario 11:

// Andrada, Reindel 
// Cabilin, Angelo James
//IDE : VSCODE
//Operating Sytem : MacOs
//Supported Operating System: MacOs, Linux, Windows
//Jan 27, 2026

//header
#include <iostream>
#include <cstdlib> // Required for system()
#include <string>
#include <cctype>
#include <iomanip> // For table formatting

//alternative for using namespace std;
using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::left;
using std::setw;

// Function Prototypes
void waitForEnter(); 
void addStudent(string studentData[][4], int &studentCount);
double computeAverage(string studentData[][4], int studentCount);
void computeSubjectAverages(string studentData[][4], int studentCount);
int findTopStudent(string studentData[][4], int studentCount);
void findTopPerSubject(string studentData[][4], int studentCount);
string getStringNoNumbers(string prompt);
int getPositiveInt(string prompt);

// Main 
int main() {
    string studentData[100][4]; // 2D array: [i][0] = name, [i][1] = Math score, [i][2] = Science score, [i][3] = English score
    int studentCount = 0;
    double average;
    int topStudentIndex;
    int choice;

    //menu
    do {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        cout << "\n----------- Menu -----------\n";
        cout << "1. Add students\n";
        cout << "2. Compute overall average\n";
        cout << "3. Compute average per subject\n";
        cout << "4. Find top student overall\n";
        cout << "5. Find top student per subject\n";
        cout << "6. Exit\n";
        choice = getPositiveInt("Choose an option: ");

        switch (choice) {
            case 1:
                addStudent(studentData, studentCount);
                break;
            case 2:
                if (studentCount == 0) {
                    cout << "No students added yet.\n";
                    waitForEnter();
                } else {
                    average = computeAverage(studentData, studentCount);
                    cout << "Overall average score: " << average << endl;
                    waitForEnter();
                }
                break;
            case 3:
                computeSubjectAverages(studentData, studentCount);
                break;
            case 4:
                if (studentCount == 0) {
                    cout << "No students added yet.\n";
                    waitForEnter();
                } else {
                    topStudentIndex = findTopStudent(studentData, studentCount);
                    double avg = 0;
                    for (int j = 1; j < 4; j++) {
                        avg += std::stoi(studentData[topStudentIndex][j]);
                    }
                    avg /= 3;
                    cout << "Top student overall: " << studentData[topStudentIndex][0] 
                         << " with average score " << avg << endl;
                    waitForEnter();
                }
                break;
            case 5:
                findTopPerSubject(studentData, studentCount);
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                waitForEnter();
        }
    } while (choice != 6);

    return 0;
}

// Function Definitions 
void waitForEnter(){
    cout << "\nPlease Enter to Continue...\n";
    string dummy;
    getline(cin, dummy);
}

//add new student
void addStudent(string studentData[][4], int &studentCount) {
    int count = getPositiveInt("How many students do you want to add? ");

    if (studentCount + count > 100) {
        cout << "Cannot add that many students. Max is 100.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        cout << "\n--- Student " << studentCount + 1 << " ---\n";

        studentData[studentCount][0] = getStringNoNumbers("Enter Student Name: ");
        int mathScore = getPositiveInt("Enter Math Score: ");
        studentData[studentCount][1] = std::to_string(mathScore);
        int scienceScore = getPositiveInt("Enter Science Score: ");
        studentData[studentCount][2] = std::to_string(scienceScore);
        int englishScore = getPositiveInt("Enter English Score: ");
        studentData[studentCount][3] = std::to_string(englishScore);

        studentCount++;
    }
}

//Get Overall Average
double computeAverage(string studentData[][4], int studentCount) {
    double sum = 0;
    int totalScores = 0;

    for (int i = 0; i < studentCount; i++) {
        for (int j = 1; j < 4; j++) { // Math, Science, English
            sum += std::stoi(studentData[i][j]);
            totalScores++;
        }
    }

    return sum / totalScores;
}

//get Subject Average
void computeSubjectAverages(string studentData[][4], int studentCount) {
    if (studentCount == 0) {
        cout << "No students added yet.\n";
        waitForEnter();
        return;
    }

    double mathSum = 0, scienceSum = 0, englishSum = 0;

    for (int i = 0; i < studentCount; i++) {
        mathSum += std::stoi(studentData[i][1]);
        scienceSum += std::stoi(studentData[i][2]);
        englishSum += std::stoi(studentData[i][3]);
    }

    cout << "\n--- Subject Averages ---\n";
    cout << left << setw(10) << "Subject" << setw(10) << "Average" << endl;
    cout << string(20, '-') << endl;
    cout << left << setw(10) << "Math" << setw(10) << (mathSum / studentCount) << endl;
    cout << left << setw(10) << "Science" << setw(10) << (scienceSum / studentCount) << endl;
    cout << left << setw(10) << "English" << setw(10) << (englishSum / studentCount) << endl;
    waitForEnter();
}

//get top student 
int findTopStudent(string studentData[][4], int studentCount) {
    int topIndex = 0;
    double maxAvg = 0;

    for (int i = 0; i < studentCount; i++) {
        double avg = 0;
        for (int j = 1; j < 4; j++) {
            avg += std::stoi(studentData[i][j]);
        }
        avg /= 3;
        if (avg > maxAvg) {
            maxAvg = avg;
            topIndex = i;
        }
    }

    return topIndex;
}

//get top student Per Subject
void findTopPerSubject(string studentData[][4], int studentCount) {
    if (studentCount == 0) {
        cout << "No students added yet.\n";
        waitForEnter();
        return;
    }

    int topMathIndex = 0, topScienceIndex = 0, topEnglishIndex = 0;

    for (int i = 1; i < studentCount; i++) {
        if (std::stoi(studentData[i][1]) > std::stoi(studentData[topMathIndex][1])) topMathIndex = i;
        if (std::stoi(studentData[i][2]) > std::stoi(studentData[topScienceIndex][2])) topScienceIndex = i;
        if (std::stoi(studentData[i][3]) > std::stoi(studentData[topEnglishIndex][3])) topEnglishIndex = i;
    }

    cout << "\n--- Top Students per Subject ---\n";
    cout << left << setw(10) << "Subject" << setw(15) << "Top Student" << setw(5) << "Score" << endl;
    cout << string(30, '-') << endl;
    cout << left << setw(10) << "Math" << setw(15) << studentData[topMathIndex][0] << setw(5) << studentData[topMathIndex][1] << endl;
    cout << left << setw(10) << "Science" << setw(15) << studentData[topScienceIndex][0] << setw(5) << studentData[topScienceIndex][2] << endl;
    cout << left << setw(10) << "English" << setw(15) << studentData[topEnglishIndex][0] << setw(5) << studentData[topEnglishIndex][3] << endl;
    waitForEnter();
}

// String input (letters only)
string getStringNoNumbers(string prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input == "") {
            cout << "Input cannot be empty.\n";
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

        cout << "Letters only. Try again.\n";
    }
}

// Positive integer
int getPositiveInt(string prompt) {
    int x;

    while (true) {
        cout << prompt;
        cin >> x;

        if (cin.fail() || x <= 0) {
            cin.clear();
            cin.ignore(1000, '\n'); // simple cleanup
            cout << "Enter a positive integer.\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}