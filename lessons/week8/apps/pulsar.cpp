#include "p5.hpp"





struct pulseCircle
{
    StopWatch sw;
    double period;

    pulseCircle(const double p = 0.25)
    :period(p)
    {sw.reset();}

    void draw()
    {
        double t = sw.seconds();
        if (t>period) {
            sw.reset();
            t = 0.0;
        }

        double x = MAP(t, 0, period, 0, HALF_PI);
        double sinx = sin(x);
        double s = MAP(sinx, 0, 1, 1, 5);

        // Calculate the alpha as the inverse of the size
        uint8_t a = MAP(s,1,5, 255,50);
        PixRGBA c = {255,0,0,a};
        fill(c);
        noStroke();
        circle(mouseX, mouseY, 20*s);
    }
};  

pulseCircle pc1(0.50);
//pulseCircle pc2(0.25);

void drawLines()
{
    for (int i = 1;i<=500;i++)
    {
        int x = MAP(mouseX, 0,width, (width/2)-200, (width/2)+200);
        stroke(color(random(255), random(255), random(255)));
        line(mouseX, mouseY, random(width), random(height));
    }
}

//void onLoop()
void draw()
{
    background(0x7f);

    drawLines();
    pc1.draw();
    //pc2.draw();
}

void setup()
{
    createCanvas(400, 400);
    frameRate(20);
}