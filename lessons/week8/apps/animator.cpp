/*
    Demonstration of some simple slide animations
*/

#include "p5.hpp"
#include "codec_targa.hpp"
#include "stopwatch.hpp"

PixelBuffer *apb = nullptr;


class TimeAnimator
{
    StopWatch clock;
    double fDuration;

protected:
    TimeAnimator(double duration)
    :fDuration(duration)
    {
        clock.reset();
    }

public:
    virtual void reset()
    {
        clock.reset();
    }

    virtual void draw()
    {}
    
    virtual bool isDone()
    {
        return clock.seconds() >= fDuration;
    }

    virtual double portion()
    {
        return constrain(clock.seconds()/fDuration, 0, 1);
    }

    virtual void update()
    {}
};

class RectAnimator : public TimeAnimator
{
    int x, y, w, h;
    PixRGBA c1;
    PixRGBA c2;
    PixRGBA c;

public:
    RectAnimator(double duration, int x, int y, int w, int h)
        :TimeAnimator(duration),
        x(x), y(y), w(w), h(h)
        {
            c1 = color(random(255), random(255), random(255));
            c2 = color(random(255), random(255), random(255));
        }

    void draw()
    {
        update();

        noStroke();
        fill(c);
        rect(x, y, w, h);
    }

    void update(){
        double p = portion();
        //printf("update, p: %f\n", p);
        c.red = MAP(p, 0, 1, c1.red, c2.red);
        c.green = MAP(p, 0, 1, c1.green, c2.green);
        c.blue = MAP(p, 0, 1, c1.blue, c2.blue);
        c.alpha = 255;
    }
};

RectAnimator rec1(3, 100, 100, 320, 200);
RectAnimator rec2(5, 250, 200, 320, 200);

void mouseReleased(const MouseEvent &e)
{
    printf("MOUSERELEASED\n");
    rec1.reset();
    rec2.reset();
}

void draw()
{
    //printf("DRAW\n");
    if (apb == nullptr) {
        return ;
    }

    //printf("DRAW 2\n");

    rec1.draw();
    rec2.draw();
}

void setup()
{

    if (gargc < 1) {
        printf("You must specify a file to display.\n");
        halt();
    }

    printf("argv[1]: %s\n", gargv[1]);

    TargaMeta meta;
    apb = readFromFile(gargv[1], meta);

    if (nullptr == apb) {
        printf("image file not loaded.");
        halt();
    }


    createCanvas(800, 600);
    
    rec1.reset();
    rec2.reset();
}