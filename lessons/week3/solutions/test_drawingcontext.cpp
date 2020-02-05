#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferRGBA32 fb(640, 480);
    DrawingContext dc(fb);

    dc.setBackground(colors.gray50);
    dc.clear();
    
    dc.fillPixel(10, 10);
    dc.fillPixel(410,10);
    dc.fillPixel(10,410);
    dc.fillPixel(410,410);



    PBM::writePPMBinary("testdc.ppm", fb);
}
