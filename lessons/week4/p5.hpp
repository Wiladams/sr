#pragma once

#include "apphost.hpp"

static size_t width = 0;
static size_t height = 0;

// Specifying a color using a 32-bit integer
// 0xAARRGGBB
class p5colors {
public:
    const PixRGBA yellow        = {0xffffff00};
    const PixRGBA cyan          = {0xff00ffff};
} p5colors;

// Canvas management
void createCanvas(size_t width, size_t height)
{
    width = width;
    height = height;
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



