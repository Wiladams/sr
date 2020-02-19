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

#include "apphost.hpp"
#include "pbm.hpp"

void clear();

bool gUseStroke = true;
bool gUseFill = true;

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
void createCanvas(size_t awidth, size_t aheight)
{
    setCanvasSize(awidth, aheight);
    gAppWindow->setCanvasSize(awidth, aheight);
    gAppWindow->show();

    // get current mouse position
    
    clear();
}

// Drawing attributes
void background(PixRGBA pix)
{
    gAppDC->setBackground(pix);
}

void fill(PixRGBA pix)
{
    gUseFill = true;
    gAppDC->setFill(pix);
}

void noFill()
{
    gUseFill = false;
    gAppDC->setFill(0x00000000);
}

void stroke(PixRGBA pix)
{
    gUseStroke = true;
    gAppDC->setStroke(pix);
}

void noStroke()
{
    gUseStroke = false;
    gAppDC->setStroke(0x00000000);
}

void clear()
{
    gAppDC->clear();
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

void rectangle(int x, int y, int width, int height)
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
    if (gUseFill) {
        gAppDC->fillEllipse(cx, cy, xRadius, yRadius);
    }

    if (gUseStroke) {
        gAppDC->strokeEllipse(cx, cy, xRadius, yRadius);
    }
}



