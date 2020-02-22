#pragma once

/*
    This file represents the interface for the P5 application
    environment.

    All the special functions, structures, and constants
    should all be located in this file.

    P5 uses the apphost routines to connect to the local window
    system to display graphics on the screen, and receive mouse
    and keyboard events.
*/

#include <stdlib.h>
#include "apphost.hpp"
#include "pbm.hpp"

/*
Global State
*/
bool gUseStroke = true;
bool gUseFill = true;

static size_t width = 0;
static size_t height = 0;


void clear();



// Specifying a color using a 32-bit integer
// 0xAARRGGBB
class p5colors {
public:
    const PixRGBA transparent   = {0x00000000};
    const PixRGBA black         = {0xff000000};
    const PixRGBA white         = {0xffffffff};

    const PixRGBA red           = {0xffff0000};
    const PixRGBA green         = {0xff00ff00};
    const PixRGBA blue          = {0xff0000ff};

    const PixRGBA yellow        = {0xffffff00};
    const PixRGBA cyan          = {0xff00ffff};

} p5colors;

// Canvas management
void createCanvas(size_t aWidth, size_t aHeight)
{
    width = aWidth;
    height = aHeight;
    
    setCanvasSize(aWidth, aHeight);

    gAppWindow->setCanvasSize(aWidth, aHeight);
    gAppWindow->show();

    // get current mouse position
    
    clear();
}

// Drawing attributes
// general color construction from components
PixRGBA color(int a, int b, int c, int d)
{
    PixRGBA pix;
    pix.red = a;
    pix.green = b;
    pix.blue = c;
    pix.alpha = d;

    return pix;
}

PixRGBA color(int r, int g, int b)
{
    return color(r, g, b, 255);
}

PixRGBA color(int gray, int alpha)
{
    return color(gray, gray, gray, alpha);
}

PixRGBA color(int gray)
{
    return color(gray,gray,gray,255);
}







void fill(PixRGBA pix, int alpha)
{
    gUseFill = true;
    PixRGBA c = pix;
    c.alpha = alpha;
    gAppDC->setFill(c);
}

void fill(PixRGBA pix)
{
    gUseFill = true;
    gAppDC->setFill(pix);
}

void noFill()
{
    gUseFill = false;
    //gAppDC->setFill(0x00000000);
}

// Setting Stroke
void stroke(PixRGBA pix)
{
    gUseStroke = true;
    gAppDC->setStroke(pix);
}

// Change the alpha of an existing color
void stroke(PixRGBA pix, int alpha)
{
    PixRGBA c = pix;
    c.alpha = alpha;
    stroke(c);
}

// create color with gray and alpha
void stroke(uint8_t gray, uint8_t alpha=255)
{
    PixRGBA c;
    c.red = gray;
    c.green = gray;
    c.blue = gray;
    c.alpha = alpha;
    stroke(c);
}

void noStroke()
{
    gUseStroke = false;
    //gAppDC->setStroke(0x00000000);
}

// Clearing the canvas
void clear()
{
    gAppDC->clear();
}

void background(PixRGBA pix)
{
    gAppDC->setBackground(pix);
}

void background(int a, int b, int c, int d)
{
    background(color(a, b, c, d));
}

void background(int gray, int alpha)
{
    background(color(gray, alpha));
}

void background(int gray)
{
    background(color(gray));
}

// Drawing Primitives
void pixel(int x1, int y1)
{
    gAppDC->fillPixel(x1, y1);
}

void line(int x1, int y1, int x2, int y2)
{
    if (!gUseStroke) { return ;}

    gAppDC->strokeLine(x1, y1, x2, y2);
}

void rect(int x, int y, int width, int height)
{
    if (gUseFill) {
        gAppDC->fillRectangle(x, y, width, height);
    }

    if (gUseStroke) {
        gAppDC->strokeRectangle(x, y, width, height);
    }
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    if (gUseStroke) {
        gAppDC->strokeTriangle(x1, y1, x2, y2, x3, y3);
    }
}

void ellipse(int cx, int cy, int xRadius, int yRadius)
{
    if ((cx < 0) || (cy < 0)) {
        return ;
    }

    if (gUseFill) {
        gAppDC->fillEllipse(cx, cy, xRadius, yRadius);
    }

    if (gUseStroke) {
        gAppDC->strokeEllipse(cx, cy, xRadius, yRadius);
    }
}


// Math functions
static const double QUARTER_PI = 0.7853982;
static const double HALF_PI = 1.57079632679489661923;
static const double PI = 3.14159265358979323846;
static const double TWO_PI = 6.28318530717958647693;
static const double TAU = 6.28318530717958647693;

// are angles specified in degrees
// or in radians
enum {
    DEGREES,
    RADIANS
};

static int gAngleMode = DEGREES;
void angleMode(int newMode)
{
    gAngleMode = newMode;
}

double degrees(double x) { return x * 57.29577951308232; }
double radians(double x) { return x * 0.017453292519943295; }

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




