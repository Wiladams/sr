#pragma once

/*
    The mating between a DIBSection and a PixelBufferRGBA32
*/

#include "w32.hpp"
#include "PixelBufferRGBA32.hpp"

class PBDIBSection
{
    BITMAPINFO fBMInfo;

public:
    PBDIBSection(size_t awidth, size_t aheight)
    {
        int bitsPerPixel = 32;
        int alignment = 4;
        int bytesPerRow = GetAlignedByteCount(awidth, bitsPerPixel, alignment)

    fBMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    fBMInfo.bmiHeader.biWidth = awidth;
    fBMInfo.bmiHeader.biHeight = -aheight;	// top-down DIB Section
    fBMInfo.bmiHeader.biPlanes = 1;
    fBMInfo.bmiHeader.biBitCount = bitsPerPixel;
    fBMInfo.bmiHeader.biSizeImage = bytesPerRow * aheight;
    fBMInfo.bmiHeader.biClrImportant = 0;
    fBMInfo.bmiHeader.biClrUsed = 0;
    fBMInfo.bmiHeader.biCompression = BI_RGB;
    

    local pPixels = ffi.new("void *[1]")
    obj.GDIHandle = CreateDIBSection(nil,
            info,
            C.DIB_RGB_COLORS,
            pPixels,
            nil,
            0);

};