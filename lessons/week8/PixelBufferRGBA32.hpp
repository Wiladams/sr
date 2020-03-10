#pragma once

#include <stdint.h>
#include <stdio.h>

#include "PixelBuffer.hpp"

/*
    This is a class that represents a framebuffer.
    A framebuffer is the most rudimentary graphics
    object.  It is meant to be displayed on the screen,
    or passed around for remote viewing, and whatnot.

    There are very few drawing routines associated with 
    the framebuffer itself.  Most routines are related
    to the PixelBuffer Object instead.

    What should be in here is the ability to blit
    a PixelBuffer, in various ways.

    The format of a FrameBuffer is 32-bit pixels.
*/
class PixelBufferRGBA32 : public PixelBuffer {
public:
    // Public constructor
    // must assign to const fields using ':'
    // mechanism.
    PixelBufferRGBA32(const size_t width, const size_t height)
        : PixelBuffer(width, height)
    {
        data = {new PixRGBA[width*height]{}};
    }

    // Virtual destructor so this can be sub-classed
    virtual ~PixelBufferRGBA32(){
        // must delete the data element, as we 
        // constructred it.
        delete [] data;
    }

    // Set the value of a single pixel
    bool setPixel(GRCOORD x, GRCOORD y, const PixRGBA &pix)
    {
        //printf("setPixel: %d %d (%d,%d,%d)\n", x, y, pix.red, pix.green, pix.blue);

        // BUGBUG - we should quick reject if alpha == 0
        if (x>= getWidth() || y >= getHeight()  || (pix.alpha == 0)) 
        {
            return false;   // outside bounds, or invisible
        }
        size_t offset = y * getWidth() + x;
        data[offset] = pix;

        return true;
    }

    // get the value of a single pixel
    // marked as 'const' because it does not alter the contents
    // of the FrameBuffer
    PixRGBA getPixel(GRCOORD x, GRCOORD y) const
    {
        //printf("getPixel: %d %d, width: %d height: %d\n", x, y, getWidth(), getHeight());

        size_t offset = y * getWidth() + x;
        //PixRGBA c = data[offset];
        //printf("getPixel: (%d,%d,%d)\n", c.red, c.green, c.blue);
        //return c;
        return this->data[offset];
    }

    // setPixels()
    // 
    // set the values of a contiguous set of pixels
    bool setSpan(GRCOORD x, GRCOORD y, const GRSIZE width, const PixRGBA * pix)
    {
        // BUGBUG - be mindful of the size of things
        // if you use someting too small, it will rollover
        // uint16_t offset = y * this->width + x;

        // size_t is a good choice, as it's typically the machine's largest
        // unsigned int
        size_t offset = y * getWidth() + x;
        
        // BUGBUG
        // we can do clipping here by reducing width to whatever
        // is remaining on the line, rather than fulfilling the
        // entire 'width' request

        for (GRSIZE i=0; i<width; i++) 
        {
            data[offset+i] = pix[i];
        }

        return true;
    }


    // setAllPixels()
    // Set all the pixels in the framebuffer to the value specified
    // This is done here in case a framebuffer has a way of doing it
    // really fast.
    bool setAllPixels(const PixRGBA value)
    {
        size_t nPixels = getWidth() * getHeight();
        while (nPixels > 0){
            data[nPixels-1] = value;
            nPixels = nPixels - 1;
        }

        return true;
    }

    // We should not do the following as it allows
    // the data pointer to escape our control
    // it also allows unrestricted access to the data itself
    // which breaks encapsulation.
    // PixRGBA * getData() const {return this->data;}
    // BUT, very convenient, and the const helps somewhat
    virtual const PixRGBA * getData() const
    {
        return data;
    }


private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    PixelBufferRGBA32();

    PixRGBA * data;         // a pointer to the actual pixel data
};
