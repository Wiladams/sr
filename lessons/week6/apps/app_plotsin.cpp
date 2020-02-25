#include "p5.hpp"
#include "plotcurve.hpp"

double SIN(double u) {return sin(MAP(u,0,1,0,radians(u*360*1)));}
double COS(double u) {return cos(MAP(u,0,1,0,radians(u*360*1)));}


void draw()
{
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
    plotcurve(SIN, 100, cmargin, cwidth, cheight, cmargin, -1.0, 1.0);

    stroke(colors.red);
    plotcurve(COS, 100, cmargin, cwidth, cheight, cmargin, -1.0, 1.0);


    noLoop();
}


void setup()
{
    createCanvas(1024,768);
    background(colors.white);
}