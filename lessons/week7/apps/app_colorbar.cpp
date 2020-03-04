#include "p5.hpp"

int barSize = 80;

void draw()
{   
    // baseline red
    background(colors.red);


    // horizontal green bar
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