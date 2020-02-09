#pragma once

#include <stdint.h>

#include "PixelBuffer.hpp"

inline uint8_t toGray(const PixRGBA pix)
{
    return (0.2125 * pix.r) + (0.7154 * pix.g) + (0.0721 * pix.b);
}
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
class PixelBufferGray : public PixelBuffer {
private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    PixelBufferGray();

    uint8_t * data;         // a pointer to the actual pixel data

public:
    // Public constructor
    // must assign to const fields using ':'
    // mechanism.
    PixelBufferGray(GRSIZE width, GRSIZE height)
        : PixelBuffer(width, height)
    {
        data = {new uint8_t[width*height]{}};
    }

    // Virtual destructor so this can be sub-classed
    virtual ~PixelBufferGray(){
        // must delete the data element, as we 
        // constructred it.
        delete [] data;
    }

    // Set the value of a single pixel
    bool setPixel(GRCOORD x, GRCOORD y, const PixRGBA pix)
    {
        if (x>= getWidth() || y >= getHeight()) 
        {
            return false;   // outside bounds
        }
        size_t offset = y * getWidth() + x;
        
        // convert pix to gray
        // use BT709 gray standard
        data[offset] = toGray(pix);

        return true;
    }

    // get the value of a single pixel
    // marked as 'const' because it does not alter the contents
    // of the FrameBuffer
    PixRGBA getPixel(GRCOORD x, GRCOORD y) const
    {
        size_t offset = y * getWidth() + x;
        uint8_t value = data[offset];
        PixRGBA pix;
        pix.a=255;
        pix.r=value;
        pix.g=value;
        pix.b=value;

        return pix;
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
            data[offset+i] = toGray(pix[i]);
        }

        return true;
    }


    // setAllPixels()
    // Set all the pixels in the framebuffer to the value specified
    // This is done here in case a framebuffer has a way of doing it
    // really fast.
    bool setAllPixels(const PixRGBA value)
    {
        uint8_t grayValue = toGray(value);

        size_t nPixels = getWidth() * getHeight();
        // BUGBUG
        // we could do this with a simple memset 
        // as an optimization
        while (nPixels > 0){
            data[nPixels-1] = grayValue;
            nPixels = nPixels - 1;
        }

        return true;
    }
};
