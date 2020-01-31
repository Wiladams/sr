#include "FrameBuffer.h"

class Render2D {
public:
    Render2D(FrameBuffer &fb)
    :fb(fb)
    {

    }

    // drawPixel(FBCOORD x, FBCOORD y, PixRGBA pix)
    // drawLine(FBCOORD x1, FBCOORD y1, FBCOORD x2, FBCOORD y2, PixRGBA pix)
    // drawHorizontalLine(FBCOORD x, FBCOORD y, FBSIZE length, PixRGBA pix)
    // drawVerticalLine(FBCOORD x, FBCOORD y, FBSIZE length, PixRGBA pix)
    // drawPolyLine()
    // drawCubicBezier()

    // drawRectangle()
    // frameRectangle()
    // fillRectangle()

    // drawRoundedRectangle()
    // frameRoundedRectangle()
    // fillRoundedRectangle()

    // drawEllipse()
    // frameEllipse()
    // frameEllipse()

    // drawTriangle()
    // frameTriangle()
    // fillTriangle()

    // drawPolygon()
    // framePolygon()
    // fillPolygon()


private:
    FrameBuffer &fb;
};