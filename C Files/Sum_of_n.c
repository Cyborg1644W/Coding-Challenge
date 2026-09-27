#include <stdio.h>
#include <math.h>

// Aliases using only x and y
#define x printf   // x = printf
#define y scanf    // y = scanf
#define xx int     // xx = int
#define xy main    // xy = main
#define yx return  // yx = return

xx xy() {
    xx x1;  // user input number
    xx x2;  // sum

    x("Enter a number: ");
    y("%d", &x1);

    x2 = (x1 + 1) * x1 / 2; // sum calculation
    x("The sum of all numbers up to %d is %d\n", x1, x2);

    yx 0;
}
