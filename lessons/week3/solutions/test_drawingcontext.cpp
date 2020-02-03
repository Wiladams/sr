#include "FrameBuffer.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    FrameBuffer fb(640, 480);
    DrawingContext dc(fb);

    dc.background(colors.gray50);

    dc.fillPixel(10, 10);
    dc.fillPixel(410,10);
    dc.fillPixel(10,410);
    dc.fillPixel(410,410);



    PBM::writePPMBinary("testdc.ppm", fb);
}
