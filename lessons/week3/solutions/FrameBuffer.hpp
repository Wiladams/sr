#pragma once

#include <stdint.h>
#include <stdio.h>

#include "grtypes.hpp"

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
    FrameBuffer(GRSIZE width, GRSIZE height)
        : width(width), height(height)
    {
        data = {new PixRGBA[width*height]{}};
    }

    // Virtual destructor so this can be sub-classed
    virtual ~FrameBuffer(){
        // must delete the data element, as we 
        // constructred it.
        delete [] data;
    }

    // Set the value of a single pixel
    bool setPixel(GRCOORD x, GRCOORD y, const PixRGBA pix)
    {
        if (x>= this->width || y>=this->height) 
        {
            return false;   // outside bounds
        }
        size_t offset = y * width + x;
        this->data[offset] = pix;

        return true;
    }

    // get the value of a single pixel
    // marked as 'const' because it does not alter the contents
    // of the FrameBuffer
    PixRGBA getPixel(GRCOORD x, GRCOORD y) const
    {
        size_t offset = y * width + x;
        return this->data[offset];
    }

    // setPixels()
    // 
    // set the values of a contiguous set of pixels
    bool setPixels(GRCOORD x, GRCOORD y, const GRSIZE width, const PixRGBA * pix)
    {
        // BUGBUG - be mindful of the size of things
        // if you use someting too small, it will rollover
        // uint16_t offset = y * this->width + x;

        // size_t is a good choice, as it's typically the machine's largest
        // unsigned int
        size_t offset = y * this->width + x;

        for (GRSIZE i=0; i<width; i++) 
        {
            this->data[offset+i] = pix[i];
        }

        return true;
    }


    // setAllPixels()
    // Set all the pixels in the framebuffer to the value specified
    // This is done here in case a framebuffer has a way of doing it
    // really fast.
    bool setAllPixels(const PixRGBA value)
    {
        for (GRSIZE row=0; row<this->height; row++) {
            for (GRSIZE col=0; col<this->width; col++) {
                size_t offset = row * this->width + col;
                this->data[offset] = value;
            }
        }

        return true;
    }

    // Retrieve various attribute
    GRSIZE getWidth() const { return this->width;}
    GRSIZE getHeight() const { return this->height;}

    // We should not do the following as it allows
    // the data pointer to escape our control
    // it also allows unrestricted access to the data itself
    // which breaks encapsulation.
    // PixRGBA * getData() const {return this->data;}

private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    FrameBuffer();

    // The width and height are const because
    // you don't want them to ever change
    const GRSIZE width;   //how many pixels wide
    const GRSIZE height;  // how many pixels tall

    PixRGBA * data;         // a pointer to the actual pixel data
    PixRGBA * scratch;      // a single row used for optimized drawing
};
