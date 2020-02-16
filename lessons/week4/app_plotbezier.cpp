#include "p5.hpp"


typedef float REAL;

// signature for a parametric function
typedef float (*CFUNC)(float u);

// Cubic bezier basis functions
// the 'u' value ranges from 0.0 to 1.0
float BEZ03(float u) {return powf(1-u, 3); }            // (1-u)^3
float BEZ13(float u) {return 3 * u*((1-u) * (1-u));}    // 3u(1-u)^2
float BEZ23(float u) {return 3 * u*u * (1-u);}          // 3u^2(1-u)
float BEZ33(float u) {return u * u*u;}                  // u^3


// Draw a parametric curve
void plotcurve(CFUNC func, int xlow, int xhigh, int ylow, int yhigh)
{
    float lastx = MAPF(0, 0,1, xlow, xhigh);
    float lasty = MAPF(func(0), 0,1, ylow, yhigh);

//printf("lastxy: %f %f\n", lastx, lasty);

    float u = 0.01;
    while (u <= 1.0) {
        float val = func(u);
        float x = MAPF(u, 0,1, xlow, xhigh);
        float y = MAPF(val, 0,1, ylow,yhigh);

        //printf("u,val: %f %f  x,y: %f %f\n", u, val, x, y);
        //printf("pixel: %d %d\n", x, y);
        
        //pixel(x, y);
        line(lastx, lasty, x, y);

        lastx = x;
        lasty = y;

        u = u + 0.01;
    }
}

/*
void onLoop()
{
    printf("ON LOOP\n");
    //noLoop();
}
*/

void draw()
{
    printf("DRAW\n");

    clear();

    int cwidth = width-10;
    int cheight = height-20;
    int cmargin = 12;

    // Draw outline
    stroke(p5colors.black);
    rectangle(5,5,cwidth, cheight);

    // plot each component of bezier curve
    // there are 4 curves
    stroke(p5colors.blue);
    plotcurve(BEZ03, cmargin, cwidth, cheight, cmargin);

    stroke(p5colors.green);
    plotcurve(BEZ13, cmargin, cwidth, cheight, cmargin);

    stroke(p5colors.red);
    plotcurve(BEZ23, cmargin, cwidth, cheight, cmargin);

    stroke(p5colors.cyan);
    plotcurve(BEZ33, cmargin, cwidth, cheight, cmargin);

    noLoop();
}

void setup()
{
    createCanvas(1024,768);
    background(p5colors.white);


    //fill(p5colors.blue);
    //plotcurve(BEZ03);
}