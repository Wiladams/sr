#pragma once

/*
    References
    https://github.com/HandmadeMath/Handmade-Math/blob/master/HandmadeMath.h
*/

#include <stdlib.h>
#include <math.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

// doesn't deal with -0 vs +0
#define ABS(a) ((a) > 0 ? (a) : -(a))

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

// Math constants
static const float  PI32 = 3.14159265359f;

static const double QUARTER_PI = 0.7853982;
static const double HALF_PI = 1.57079632679489661923;
static const double PI = 3.14159265358979323846;
static const double TWO_PI = 6.28318530717958647693;
static const double TAU = 6.28318530717958647693;

// Math functions
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

/*
    Some linear algebra stuff
*/
/*
// A 2D vector type to be used to store stuff
// and do transformations.
    This is done as a union for the convenience of getting at 
    the fields with with x,y or width/height names, as well
    as using array style indexing.

    The operator overloads make for convenient arithmetic
    operations.
*/
typedef union vec2 {
    struct { double x, y;};
    struct { double width, height;};
    struct { double u, v;};

    double Elements[2];

    vec2(double a, double b) :x(a),y(b){}

    // convenient operator overloading
    // using indexing
    inline double &operator[](const int &idx) { return Elements[idx];}

    // Arithmetic Altering self
    vec2 operator+(const vec2 &b) const {return vec2(x+b.x,y+b.y);}
    vec2 operator+(double num) const {return vec2(x+num, y+num);}
    vec2 operator-(const vec2 &b) const {return vec2(x-b.x,y-b.y);}
    vec2 operator-(double num) const {return vec2(x-num, y-num);}
    vec2 operator*(double num) const {return vec2(x*num, y*num);}
    vec2 operator/(double num) const {return vec2(x/num, y/num);}

    // Aritmetic Assignment operators
    vec2& operator += (const vec2 &b) { x = x + b.x; y=y+b.y; return *this;}
    vec2& operator -= (const vec2 &b) { x = x - b.x; y=y-b.y; return *this;}
    vec2& operator *= (const vec2 &b) { x = x * b.x; y=y*b.y; return *this;}
    vec2& operator *= (const double num) { x = x * num; y=y*num; return *this;}

} vec2;


typedef union vec3 {
    struct { double x, y, z;};
    struct { double u, v, w;};
    struct { double red, green, blue;};


    double Elements[3];

    vec3(double a, double b, double c) :x(a),y(b), z(c){}

    // convenient operator overloading
    // using indexing
    inline double &operator[](const int &idx) { return Elements[idx];}

    // Arithmetic Altering self
    vec3 operator+(const vec3 &b) const {return vec3(x+b.x,y+b.y, z+b.z);}
    vec3 operator+(double num) const {return vec3(x+num, y+num, z+num);}
    vec3 operator-(const vec3 &b) const {return vec3(x-b.x,y-b.y, z-b.z);}
    vec3 operator-(double num) const {return vec3(x-num, y-num, z-num);}
    vec3 operator*(double num) const {return vec3(x*num, y*num, z*num);}
    vec3 operator/(double num) const {return vec3(x/num, y/num, z/num);}

    // Aritmetic Assignment operators
    vec3& operator += (const vec3 &b) { x = x + b.x; y=y+b.y; z=z+b.z; return *this;}
    vec3& operator -= (const vec3 &b) { x = x - b.x; y=y-b.y; z=z-b.z; return *this;}
    vec3& operator *= (const vec3 &b) { x = x * b.x; y=y*b.y; z=z*b.z; return *this;}
    vec3& operator *= (const double num) { x = x * num; y=y*num; z=z*num; return *this;}

} vec3;
