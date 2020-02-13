/*
    Using apphost makes programming look very similar to P5.js.

    A general window is already setup.  You can resize the canvas.
    Mouse and keyboard handling is as simple as implementing the
    appropriate routines.
*/
#include "p5.hpp"


void draw()
{
    if ((width == 0) || (height == 0)) {
        return ;
    }

    static int posx = 0;
    static int posy = 0;
    static int rectSize = 200;
    static int xdir = 1;

    posx = posx+3*xdir;
    if (posx >= width-rectSize-3) {
        posx = 0;
    }
    posy = posy +2;
    if (posy >= height - rectSize-2) {
        posy = 0;
    }

//printf("draw: %zd %zd %d %d\n", width, height, posx, posy);

    clear();

    fill(p5colors.yellow);
    rectangle(posx,posy, rectSize,rectSize);
}

void setup()
{
    printf("SETUP\n");
    createCanvas(640, 480);
    background(p5colors.cyan);
}
