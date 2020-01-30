#include "FrameBuffer.h"
#include "colors.h"
#include "pbm.h"

void main()
{
    FrameBuffer fb(1024, 768);

    fb.setAllPixels(colors.gray50);

    fb.setRectanglePixels(10, 110, 300, 200, colors.red);
    fb.setRectanglePixels(110, 210, 300, 200, colors.green);
    fb.setRectanglePixels(210, 310, 300, 200, colors.blue);
    
    PBM::writePPMBinary("fbrectangle.ppm", fb);
}