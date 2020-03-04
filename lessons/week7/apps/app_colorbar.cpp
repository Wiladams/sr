#include "p5.hpp"

void draw()
{   
    PixRGBA c1(255,0,0,255);
    PixRGBA c2(0,255,0,200);
    PixRGBA c3 = color(0,0,255,127);

    // baseline red
    stroke(c1);
    for (int y = 0; y<250; y++) {
        line(0,y,width-1,y);
    }

    // overlapping translucent green
    stroke(c2);
    for (int y = 150; y<height-1; y++) {
        line(0,y,width-1,y);
    }

    // overlapping translucent blue
    stroke(c3);
    for (int x=180;x<240;x++) {
        line(x,0,x,height-1);
    }
}

void setup()
{
    createCanvas(400,400);
    background(0xc0);
}