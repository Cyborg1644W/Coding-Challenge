#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::string;

void getName(int length){
    string name;
    do{
        cout << "Enter Your Name: ";
        getline(cin, name);

        if (name.length() > length){
            cout << "Too Many Characters\n";
        }
        else if(name.length() == 0){
            cout << "Name cannot be empty\n";
        } else {
            cout << "Hello " << name;
        }
    } while (name.length() == 0 || name.length() > length);
    
}

string makeGmail(string name) {
    name = name.append("@gmail.com");

    return name;
}

string makeUserName(string name) {
    name = name.insert(0, "@");

    return name;
}

char getCharUsingIndex(string name, int index) {
    char Character = name.at(index);
    
    return Character;
}

int findCharacter(string name, string finds) {
    int index = name.find(finds);

    return index;
}

string eraseCharacters(string name,int startIndex, int endIndex) {
    string updatedName = name.erase(startIndex, endIndex);

    return updatedName;
}

int main(){
    
    return 0;
}