/*
    Using apphost makes programming look very similar to P5.js.

    A general window is already setup.  You can resize the canvas.
    Mouse and keyboard handling is as simple as implementing the
    appropriate routines.
*/
#include "p5.hpp"


void draw()
{
    static posx = 0;
    static posy = 0;
    static rectSize = 200;

    clear();

    fill(p5colors.yellow);
    rectangle(320,240, rectSize,rectSize);
}

void setup()
{
    printf("SETUP\n");
    createCanvas(640, 480);
    background(p5colors.cyan);
}
