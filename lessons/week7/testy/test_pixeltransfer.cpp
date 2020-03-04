#include "p5.hpp"
#include <stdio.h>

void printColor(const PixRGBA c)
{
    printf("rgba: %d %d %d %d\n", c.red, c.green, c.blue, c.alpha);
}

void draw()
{
    PixRGBA c1 = color(255,0,0,255);
    PixRGBA c2 = color(0,255,0,200);

    //printColor(c1);
    //printColor(c2);
    stroke(c1);
    line(10,10,20,10);


    set(10,10,c2);

    noLoop();
}

void setup()
{
    createCanvas(400,400);
}