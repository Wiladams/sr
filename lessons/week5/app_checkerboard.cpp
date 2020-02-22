#include "p5.hpp"
#include "checkerboard.hpp"

Checkerboard cb(640,480,16,16);

void draw()
{
    cb.draw();

    noLoop();
}

void setup()
{
    createCanvas(640,480);
}