#include "p5.hpp"

/*
    In this application, there are two things of interest.
    The first is that color bars are created and move 
    around the screen tracking the mouse.

    A red background is drawn
    Then a horizontal green bar follows the mouse up and
    down the screen.

    A blue vertical bar moves from left to right with the 
    mouseX location.

    Since the default pixel transfer operation is SRCOVER
    the color's alpha value is changed based on the position
    of the mouse, vertically or horizontally.
*/

int barSize = 80;

void draw()
{   
    // baseline red
    background(colors.red);


    // horizontal green bar
    // set the alpha of the color based on the vertical
    // position of the mouseY value.  It will be 20 when
    // the mouse is at the top of the window, and it will
    // be 255 when the mouse is at the bottom of the window
    PixRGBA c2 = color(0,255,0,MAP(mouseY, 0,height-1, 20,255));
    fill(c2);
    noStroke();
    int y = constrain(mouseY, 0, height-barSize);
    rect(0,y, width-1,barSize);


    // vertical blue bar
    PixRGBA c3 = color(0,0,255,MAP(mouseX, 0,width-1, 20,255));
    noStroke();
    fill(c3);
    int x = constrain(mouseX, 0, width-barSize);
    rect(x,0,barSize,height);

}

void setup()
{
    createCanvas(400,400);
}