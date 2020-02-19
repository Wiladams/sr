#include "p5.hpp"

int radius = 50;

/*
void mouseMoved(const MouseEvent &e)
{
    printf("mouse: %d %d\n", e.x, e.y);
}
*/

void draw()
{
    clear();

    ellipse(mouseX, mouseY, radius, radius);
}

void setup()
{
    createCanvas(640,480);
    setFrameRate(15);
}