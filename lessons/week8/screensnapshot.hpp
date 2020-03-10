#pragma once

/*
    Take a snapshot of the video display using GDI
*/
#include "w32.h"
#include "PixelBufferRGBA32.hpp"

class ScreenSnapshot
{
    GDIContext fContext;
    PixelBufferRGBA32 fImage;
    HDC fScreenDC;
    HDC fImageDC;
    int fOriginX;
    int fOriginY;
    size_t fWidth;
    size_t fHeight;

    ScreenSnapshot(int x, int y, size_t awidth, size_t aheight)
        : fImage(awidth, aheight),
        fWidth(awidth),
        fHeight(aheight)
    {
        // create a pixel buffer of the specified size
        fScreenDC = CreateDCA("DISPLAY", nullptr, nullptr, nullptr);
        
        // Create memory device context
        fImageDC = CreateCompatibleDC(fScreenDC);
        // Create DIB Section
        // select fImageDC into DIB Section




    }

    void reset() {
        // do nothing
    }

    PixelBufferRGBA32 & getCurrent() 
    {
        return fImage;
    }

    bool moveNext()
    {
        GDIDIBSection contextImage = new GDIDIBSection(rect.Width, rect.Height, BitCount.Bits24);

        BitBlt(fContext, new Point(rect.X, rect.Y), new Rectangle(0, 0, rect.Width, rect.Height),
                (TernaryRasterOps.SRCCOPY | TernaryRasterOps.CAPTUREBLT));

        return true;
    }

}