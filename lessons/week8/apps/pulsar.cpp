#include "p5.hpp"

StopWatch sw;

//void onLoop()
void draw()
{
    background(0x7f);

    double scale = 1.0;

    double t = sw.seconds();
    if (t>0.25) {
        sw.reset();
        t = 0.0;
    }

    double x = MAP(t, 0, 0.25, 0, HALF_PI);
    double sinx = sin(x);
    double s = MAP(sinx, 0, 1, 1, 5);

    stroke(0);
    for (int i = 1;i<=1000;i++)
    {
        line(random(width),random(height), random(width),random(height));
    }

    noStroke();
    fill(colors.blue);
    rect(10,10,20,20*s);

    uint8_t a = MAP(s,1,5, 255,50);
    PixRGBA c = {255,0,0,a};
    fill(c);
    //rect(mouseX, mouseY, 20*s, 20*s);
    circle(mouseX, mouseY, 20*s);
}

void setup()
{
    sw.reset();
    createCanvas(400, 400);
    frameRate(30);
}