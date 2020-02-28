#include "p5.hpp"
#include "collections.hpp"
#include "checkerboard.hpp"
#include "ToolSelector.hpp"
#include "ColorSelector.hpp"

static const int edgeMargin = 10;

static const int interMargin = 4;
static const int canvasWidth = 1024;
static const int canvasHeight =768;

PixRGBA  randomColor()
{
    int r = random(30,255);
    int g = random(30,255);
    int b = random(30,255);
    
    return PixRGBA(r,g,b,126);
}








ToolSelector ts(edgeMargin, edgeMargin);
ColorSelector cs(edgeMargin + toolWidth + interMargin, canvasHeight-64-10);

void draw()
{
    background(0xC0);

    ts.draw();
    cs.draw();
}

void setup()
{
    createCanvas(canvasWidth, canvasHeight);
}