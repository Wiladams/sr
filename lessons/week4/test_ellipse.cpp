/*
    In this case, we exercise various of the rectangle drawing routines.
*/

#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferRGBA32 fb(640, 480);
    DrawingContext dc(fb);

    dc.clear();

    // default white fill color
    dc.drawEllipse(320,240, 200,200);

    PBM::writePPMBinary("testellipse.ppm", fb);
}
