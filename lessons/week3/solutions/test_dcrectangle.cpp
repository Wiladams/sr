/*
    In this case, we exercise various of the rectangle drawing routines.
*/

#include "FrameBuffer.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    FrameBuffer fb(640, 480);
    DrawingContext dc(fb);

    dc.clear();

    // default white fill color
    dc.drawRectangle(10,10, 400,400);

    dc.setFill(colors.blue);
    dc.fillRectangle(64,64, 300, 300);

    dc.setFill(colors.green);
    dc.fillRectangle(104, 84, 64, 64);
    dc.fillRectangle(260, 84, 64, 64);
    dc.fillRectangle(196, 200, 48, 64);

    PBM::writePPMBinary("testdcrect.ppm", fb);
}
