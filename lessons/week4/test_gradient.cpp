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
    // To show inheritance and polymorphism work
    // properly, you should be able to use either 
    // of these pixel buffers without any problem
    //PixelBufferRGBA32 pb(640, 480);

    PixelBufferGray pb(640, 480);
    DrawingContext dc(pb);

    dc.clear();

    // default white fill color
    for (int i=0;i<200;i++) {
        PixRGBA pix;
        pix.r = MAPI(i, 0,200, 0,255);
        pix.g = MAPI(i,0, 200, 0,64);
        pix.b = 0;
        dc.setStroke(pix);
        dc.strokeLine(i,0,i,pb.getHeight()-1);
    }

    for (int i=0;i<200;i++) {
        PixRGBA pix;
        pix.r = MAPI(i, 0,200, 255,0);
        pix.g = MAPI(i, 0,200, 64,255);
        pix.b = MAPI(i, 0,200,0,64);
        dc.setStroke(pix);
        dc.strokeLine(200+i,0,200+i,pb.getHeight()-1);
    }

    for (int i=0;i<240;i++) {
        PixRGBA pix;
        pix.r = 0;
        pix.g = MAPI(i,0,240,255,0);
        pix.b = MAPI(i,0,240,64,255);
        dc.setStroke(pix);
        dc.strokeLine(400+i,0,400+i,pb.getHeight()-1);
    }
    PBM::writePPMBinary("testgraygradient.ppm", pb);
}