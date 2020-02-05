#include "PixelBufferRGBA32.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void main()
{
    PixelBufferRGBA32 fb(640, 640);
    DrawingContext dc(fb);

    dc.setBackground(colors.white);
    dc.clear();

    // default black stroke
    dc.setStroke(colors.blue);
    for (int row=0; row<639;row+=3) {
        dc.strokeHorizontalLine(0,row,row);
    }

    dc.setStroke(colors.red);
    for (int col=0; col<639;col+=3) {
        dc.strokeVerticalLine(col,0,col);
    }

    PBM::writePPMBinary("testdchorzline.ppm", fb);
}
