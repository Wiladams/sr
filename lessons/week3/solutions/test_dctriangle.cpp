#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferRGBA32 fb(640, 480);
    DrawingContext dc(fb);

    GRSIZE width = fb.getWidth();
    GRSIZE height = fb.getHeight();
    GRSIZE halfwide = width / 2;
    GRSIZE halfhigh = height/2;

    dc.setBackground(colors.white);
    dc.clear();

    dc.setStroke(colors.blue);
    GRTriangle geo1 = {300,20,400,100,100,200};
    dc.strokeTriangle(geo1);


    PBM::writePPMBinary("testdctriangle.ppm", fb);
}
