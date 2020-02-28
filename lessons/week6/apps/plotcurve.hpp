#pragma once

#include "maths.hpp"

// signature for a parametric function
typedef double (*CFUNC)(double u);



// Draw a parametric curve
void plotcurve(CFUNC func, int nSegs, double xlow, double xhigh, double ylow, double yhigh, double rlow=0.0, double rhigh=1.0)
{
    double lastx = MAP(0, 0,1, xlow, xhigh);
    double lasty = MAP(func(0), rlow,rhigh, ylow, yhigh);

    double incr = 1.0f/nSegs;

//printf("lastxy: %f %f\n", lastx, lasty);

    double u = incr;
    while (u <= 1.0) {
        double val = func(u);
        double x = MAP(u, 0,1, xlow, xhigh);
        double y = MAP(val, rlow,rhigh, ylow,yhigh);

        //printf("u,val: %f %f  x,y: %f %f\n", u, val, x, y);
        //printf("pixel: %d %d\n", x, y);
        
        //pixel(x, y);
        line(lastx, lasty, x, y);

        lastx = x;
        lasty = y;

        u = u + incr;
    }
}