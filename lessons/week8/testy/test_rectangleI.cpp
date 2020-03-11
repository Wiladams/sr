#include "p5.hpp"
#include "RectangleI.hpp"

void draw()
{
    RectangleI r1(10,10, 100, 100);
    RectangleI r2(30, 30, 100, 100);
    RectangleI r3 = RectangleI::intersection(r1, r2);
    
    noStroke();
    fill(colors.blue);
    rect(r1.x1, r1.y1, r1.getWidth(), r1.getHeight());

    fill(colors.green);
    rect(r2.x1, r2.y1, r2.getWidth(), r2.getHeight());

    fill(colors.red);
    rect(r3.x1, r3.y1, r3.getWidth(), r3.getHeight());
    
    noLoop();
}

void setup()
{
    createCanvas(800,600);
} 