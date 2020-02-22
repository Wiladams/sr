#pragma once

#include "grtypes.hpp"
#include "p5.hpp"

// signature for a parametric function
// A parametric function is one that takes as input a
// value that ranges from 0.0 to 1.0 and returns another
// value.  Simple things like cubic bezier, and the like
//
// Realistically, it can be any range of values that are
// passed in.  We just need a signature that takes a REAL
// and returns a REAL.
typedef float (*PARAMFUNC)(float u);

// Draw a parametric curve
void plotparametric(PARAMFUNC func, double xlow, double xhigh, double ylow, double yhigh)
{
    float lastx = MAP(0, 0,1, xlow, xhigh);
    float lasty = MAP(func(0), 0,1, ylow, yhigh);

//printf("lastxy: %f %f\n", lastx, lasty);

    REAL u = 0.01;
    while (u <= 1.0) {
        float val = func(u);
        float x = MAP(u, 0,1, xlow, xhigh);
        float y = MAP(val, 0,1, ylow,yhigh);

        //printf("u,val: %f %f  x,y: %f %f\n", u, val, x, y);
        //printf("pixel: %d %d\n", x, y);
        
        //pixel(x, y);
        line(lastx, lasty, x, y);

        lastx = x;
        lasty = y;

        u = u + 0.01;
    }
}
