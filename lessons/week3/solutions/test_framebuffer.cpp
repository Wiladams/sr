#include "FrameBuffer.h"
#include "colors.h"
#include "pbm.h"

void main()
{
    FrameBuffer fb(640, 480);

    fb.setPixel(10,10,colors.red);
    fb.setAllPixels(colors.gray50);

    PBM::writePPMBinary("testfb.ppm", fb);
}
