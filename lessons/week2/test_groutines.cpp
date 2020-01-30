#include "groutines.h"

#include <stdlib.h>
#include <stdio.h>

void main()
{
    // allocate an instance of the structure on the stack
    PixelBuffer pb;

    // initialize it
    bool success = PB_init(pb, 320, 240);

    // draw some stuff
    PixRGBA yellow;
    yellow.r = 255;
    yellow.g = 255;
    
    PixRGBA red; 
    red.r = 255;

    PB_HorizontalLine(pb, 5, 5, 100, yellow);
    PB_Rectangle(pb, 10, 10, 200, 200, red);

    write_PPM_binary("testfile.ppm", pb);	// write a P6 file

    // clear the thing up
    PB_clear(pb);
}