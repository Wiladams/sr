#pragma once

#include <math.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

inline double MAP(double x, double olow, double ohigh, double rlow, double rhigh) 
{
    return (double)(rlow + (x-olow)*((double)(rhigh-rlow)/(ohigh-olow)));
}

// Some useful routines
// returns the sign of the value
// value  < 0 --> -1
// value  > 0 -->  1
// value == 0 -->  0
// this will only work in cases where 0 represents false
// and 1 represents true
inline int sgn(double val) { return ((0 < val) - (val < 0)); }

inline double sq(double x) {return x*x;}

#define swap16(a, b) { int16_t t = a; a = b; b = t; }

// Math functions
static const double QUARTER_PI = 0.7853982;
static const double HALF_PI = 1.57079632679489661923;
static const double PI = 3.14159265358979323846;
static const double TWO_PI = 6.28318530717958647693;
static const double TAU = 6.28318530717958647693;

double degrees(double x) { return x * 57.29577951308232; }
double radians(double x) { return x * 0.017453292519943295; }

double constrain(double x, double low, double high)
{
    return MIN(MAX(x, low), high);
}

double dist(double x1, double y1, double x2, double y2)
{
    return sqrt(sq(x2-x1) + sq(y2-y1));
}

double lerp(double low, double high, double x)
{
    return low + x*(high-low);
}

double mag(double x, double y)
{
    return sqrt(x*x +y*y);
}

double random()
{
    return (double)rand()/(double)RAND_MAX;
}

double random(double low, double high)
{
    return MAP(random(), 0,1, low,high);
}

double random(double high)
{
    return random(0, high);
}


