#include "groutines.h"

#include <stdlib.h>


GPROC_EXPORT bool PB_init(PixelBuffer &pb, const size_t width, const size_t height)
{
    pb.height = height;
    pb.width = width;
    pb.pixelpitch = pb.width;

    pb.data = (PixRGBA *)malloc(sizeof(PixRGBA)*width*height);

    return true;
}

GPROC_EXPORT bool PB_clear(PixelBuffer &pb)
{
    free(pb.data);
    pb.height = 0;
    pb.width = 0;
    pb.pixelpitch = 0;

    return true;
}

GPROC_EXPORT bool PB_setPixel(PixelBuffer &pb, size_t x, size_t y, PixRGBA c)
{
    size_t offset = (y * pb.pixelpitch) + x;
    pb.data[offset] = c;

    return true;
}

GPROC_EXPORT bool PB_HorizontalLine(PixelBuffer &pb, 
    const size_t row, const size_t col, const size_t numberOfPixels, 
    const PixRGBA c)
{
    int idx = col;
    while (idx < col+numberOfPixels)
    {
        PB_setPixel(pb, idx, row, c);
        idx = idx + 1;
    }

    return true;
}

GPROC_EXPORT bool PB_Rectangle(PixelBuffer &pb, size_t x, size_t y, 
    size_t width, size_t height, const PixRGBA c)
{
    int row = y;
    while (row < y  + height) {
        PB_HorizontalLine(pb, row, x, width, c);
        row = row + 1;
    } 

    return true;
}
