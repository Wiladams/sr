#include "p5.hpp"
#include "plotcurve.hpp"

/*
    Create a plot that shows the basis
    functions for a cubic bezier curve
    There are 4 separate functions
*/

// Cubic bezier basis functions
// the 'u' value ranges from 0.0 to 1.0
double BEZ03(double u) {return powf(1-u, 3); }            // (1-u)^3
double BEZ13(double u) {return 3 * u*((1-u) * (1-u));}    // 3u(1-u)^2
double BEZ23(double u) {return 3 * u*u * (1-u);}          // 3u^2(1-u)
double BEZ33(double u) {return u * u*u;}                  // u^3


void draw()
{
    //printf("DRAW\n");

    background(colors.white);

    int cwidth = width-10;
    int cheight = height-20;
    int cmargin = 12;

    // Draw outline
    stroke(colors.black);
    rect(5,5,cwidth, cheight);

    // plot each component of bezier curve
    // there are 4 curves
    stroke(colors.blue);
    plotcurve(BEZ03, cmargin, cwidth, cheight, cmargin);

    stroke(colors.green);
    plotcurve(BEZ13, cmargin, cwidth, cheight, cmargin);

    stroke(colors.red);
    plotcurve(BEZ23, cmargin, cwidth, cheight, cmargin);

    stroke(colors.cyan);
    plotcurve(BEZ33, cmargin, cwidth, cheight, cmargin);

    noLoop();
}

void setup()
{
    createCanvas(1024,768);
    background(colors.white);
}