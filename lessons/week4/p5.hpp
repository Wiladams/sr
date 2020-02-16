#pragma once

#include "apphost.hpp"

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
void createCanvas(size_t awidth, size_t aheight)
{
    setCanvasSize(awidth, aheight);
    gAppWindow->setCanvasSize(awidth, aheight);
    gAppWindow->show();
    
    clear();
}

// Drawing attributes
void fill(PixRGBA pix)
{
    gAppDC->setFill(pix);
}

void background(PixRGBA pix)
{
    gAppDC->setBackground(pix);
}

void stroke(PixRGBA pix)
{
    gAppDC->setStroke(pix);
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
    gAppDC->strokeLine(x1, y1, x2, y2);
}

void rectangle(int x, int y, int width, int height)
{
    gAppDC->drawRectangle(x, y, width, height);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    gAppDC->strokeTriangle(x1, y1, x2, y2, x3, y3);
}

void ellipse(int cx, int cy, int xRadius, int yRadius)
{
    gAppDC->drawEllipse(cx,cy, xRadius,yRadius);
}



