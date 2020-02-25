#include "p5.hpp"

void draw()
{
    background(0xc0);
    
    stroke(0);
    // vertical lines
    for (int x=20; x<width; x+=20) {
        line(x, 0,x,height-1);
    }

    // horizontal lines
    for (int y=20; y<height; y+=20) {
        line(0, y,width-1,y);
    }

    fill(colors.yellow);
    noStroke();
    rect(21,21,19,19);
}

void setup()
{
    createCanvas(320, 240);
}