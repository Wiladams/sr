/*
    Using apphost makes programming look very similar to P5.js.

    A general window is already setup.  You can resize the canvas.
    Mouse and keyboard handling is as simple as implementing the
    appropriate routines.
*/
#include "apphost.hpp"


void mouseClicked(const MouseEvent &e)
{
    PixelBufferRGBA32 fb(640, 480);
    DrawingContext dc(fb);

    dc.clear();

    // default white fill, black stroke
    dc.drawEllipse(320,240, 200,200);

    // Whenever the user clicks the mouse
    // write the image out to a file
    PBM::writePPMBinary("appellipse.ppm", fb);
}

void setup()
{
    printf("SETUP\n");
    
    //createCanvas(640, 480);
}
