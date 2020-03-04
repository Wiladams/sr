#include "p5.hpp"

void draw()
{
    ellipse(mouseX, mouseY, 20, 20);
}

void setup()
{
    createCanvas(400,400);
    background(0xC0);
}