#pragma once

#include <stdint.h>
#include <stdio.h>

#include "grtypes.h"

typedef uint16_t FBCOORD;

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
class FrameBuffer {
public:
    // Public constructor
    // must assign to const fields using ':'
    // mechanism.
    FrameBuffer(uint16_t width, uint16_t height)
        : width(width), height(height)
    {

        data = {new PixRGBA[width*height]{}};
        scratch = {new PixRGBA[width]{}};
    }

    // Virtual destructor so this can be sub-classed
    virtual ~FrameBuffer(){
        // must delete the data element, as we 
        // constructred it.
        delete [] data;
        delete [] scratch;
    }

    // Set the value of a single pixel
    bool setPixel(uint16_t x, uint16_t y, PixRGBA pix)
    {
        size_t offset = y * width + x;
        this->data[offset] = pix;

        return true;
    }

    // get the value of a single pixel
    // marked as 'const' because it does not alter the contents
    // of the FrameBuffer
    PixRGBA getPixel(uint16_t x, uint16_t y) const
    {
        size_t offset = y * width + x;
        return this->data[offset];
    }

    // set the values of an array of pixels
    bool setPixels(uint16_t x, uint16_t y, const size_t width, const PixRGBA * pix)
    {
        // BUGBUG - be mindful of the size of things
        // if you use someting too small, it will rollover
        // uint16_t offset = y * this->width + x;

        // size_t is a good choice, as it's typically the machine's largest
        // unsigned int
        size_t offset = y * this->width + x;

        //printf("setPixels: x: %d y: %d width: %d  offset: %d\n", x, y, width, offset);

        for (size_t i=0; i<width; i++) 
        {
            this->data[offset+i] = pix[i];
        }

        return true;
    }

    bool setRectanglePixels(uint16_t x, uint16_t y, 
        uint16_t width, uint16_t height, const PixRGBA pix)
    {
        // Fill up a single row of pixels
        //PixRGBA *arow = {new PixRGBA[width]{}};
        for (size_t idx=0;idx < width;idx++)
        {
            scratch[idx] = pix;
        }

        // Use that single row to fill up a whole rectangle
        for (uint16_t idx = 0; idx<height; idx++)
        {
            setPixels(x, y+idx, width, scratch);
        }

        // cleanup that one row we created
        //delete[] arow;

        return true;
    }


    // clear()
    // Set all the pixels in the framebuffer to the value specified
    bool setAllPixels(const PixRGBA value)
    {
        for (size_t row=0; row<this->height; row++) {
            for (size_t col=0; col<this->width; col++) {
                size_t offset = row * this->width + col;
                this->data[offset] = value;
            }
        }

        return true;
    }

    // Retrieve attributes
    uint16_t getWidth() const { return this->width;}
    uint16_t getHeight() const { return this->height;}
    PixRGBA * getData() const {return this->data;}

private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    FrameBuffer();

    // The width and height are const because
    // you don't want them to ever change
    const uint16_t width;   //how many pixels wide
    const uint16_t height;  // how many pixels tall

    PixRGBA * data;         // a pointer to the actual pixel data
    PixRGBA * scratch;      // a single row used for optimized drawing
};
