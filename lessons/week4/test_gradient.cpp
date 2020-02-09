/*
    In this case, we exercise various of the rectangle drawing routines.
*/

#include "PixelBufferGray.hpp"
#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferGray pb(640, 480);
    //PixelBufferRGBA32 pb(640, 480);
    DrawingContext dc(pb);

    dc.clear();

    // default white fill color
    for (int i=0;i<200;i++) {
        PixRGBA pix;
        pix.r = i;
        pix.g = 0;
        pix.b = 0;
        dc.setStroke(pix);
        dc.strokeLine(i,0,i,pb.getHeight()-1);
    }

    for (int i=0;i<200;i++) {
        PixRGBA pix;
        pix.r = 240;
        pix.g = i;
        pix.b = 0;
        dc.setStroke(pix);
        dc.strokeLine(200+i,0,200+i,pb.getHeight()-1);
    }

    for (int i=0;i<240;i++) {
        PixRGBA pix;
        pix.r = 240;
        pix.g = 240;
        pix.b = i;
        dc.setStroke(pix);
        dc.strokeLine(400+i,0,400+i,pb.getHeight()-1);
    }
    PBM::writePPMBinary("testgraygradient.ppm", pb);
}