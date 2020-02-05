#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferRGBA32 fb(1920, 1280);
    DrawingContext dc(fb);

    GRSIZE width = fb.getWidth();
    GRSIZE height = fb.getHeight();
    GRSIZE halfwide = width / 2;
    GRSIZE halfhigh = height/2;

    dc.setBackground(colors.white);
    dc.clear();

    dc.setStroke(colors.blue);
    for (int cnt=0; cnt<width-1;cnt+=3) {
        dc.strokeLine(halfwide,halfhigh,cnt,0);
    }

    dc.setStroke(colors.green);
    for (int cnt=1; cnt<width-1;cnt+=3) {
        dc.strokeLine(halfwide,halfhigh,cnt,0);
    }

    dc.setStroke(colors.red);
    for (int cnt=2; cnt<width-1;cnt+=3) {
        dc.strokeLine(halfwide,halfhigh,cnt,0);
    }

    PBM::writePPMBinary("testdcline.ppm", fb);
}
