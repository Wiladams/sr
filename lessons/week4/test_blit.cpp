/*
    virtual bool blit(PixelBuffer &src, int srcX, int srcY, int srcWidth, int srcHeight, 
        int destX, int destY, 
        int destWidth, int destHeight)
*/

#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"


void main()
{
    // create pixel buffer and image to be copied
    PixelBufferRGBA32 ib(100, 100);
    DrawingContext ibdc(ib);
    ibdc.setBackground(colors.transparent);
    ibdc.setFill(colors.green);
    ibdc.clear();

    // default white fill color
    ibdc.fillEllipse(49,49, 50,50);


    // Create pixel buffer we'll draw into
    PixelBufferRGBA32 pb(640, 480);
    DrawingContext dc(pb);
    dc.clear();

    for (int i=1;i<10;i++){
        int destX = i*20;
        int destY = i*20;
        int destWidth = MAPI(i, 1,10, 50,440);
        int destHeight = MAPI(i,1,10, 50,240);

        pb.blit(ib, 0, 0, ib.getWidth(), ib.getHeight(), destX, destY, destWidth, destHeight);
    }

    PBM::writePPMBinary("testblit.ppm", pb);
}