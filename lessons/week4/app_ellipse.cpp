/*
    In this case, we exercise various of the rectangle drawing routines.
*/


#include "apphost.hpp"

void setup()
{
    PixelBufferRGBA32 fb(640, 480);
    DrawingContext dc(fb);

    dc.clear();

    // default white fill color
    dc.drawEllipse(320,240, 200,200);

    PBM::writePPMBinary("appellipse.ppm", fb);
}
