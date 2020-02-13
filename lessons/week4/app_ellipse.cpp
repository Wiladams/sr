/*
    Using apphost makes programming look very similar to P5.js.

    A general window is already setup.  You can resize the canvas.
    Mouse and keyboard handling is as simple as implementing the
    appropriate routines.
*/
#include "apphost.hpp"


void draw()
{
    gAppDC->clear();

    // default white fill, black stroke
    gAppDC->drawEllipse(320,240, 200,200);
}

void setup()
{
    printf("SETUP\n");
    //createCanvas(640, 480);
    gAppDC->setBackground(colors.cyan);
}
