#include <iostream>

using std::endl;
using std::cout;

class Rectangle {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) {
        length = l;
        width = w;
    }
    
    double computeArea() {
        return length * width;
    }
};

int main() {
    Rectangle myRectangle(104, 43);
    
    double area = myRectangle.computeArea();
    
    cout << "The area of rectangle is " << area << endl;
    return 0;
}