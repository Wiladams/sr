#pragma once

#include <stdint.h>
#include <stdio.h>

#include "grtypes.hpp"

/*
    This base class essentially defines an interface
    for the lowest level drawing API on a buffer of pixels.

    The virtual destructor, plus the pure virtual methods
    ensure you can not create an instance of one of these,
    but you can use it as a parameter in function calls.
*/

class PixelBuffer
{
public:
    PixelBuffer(GRCOORD awidth, GRCOORD aheight)
        :width(awidth), height(aheight)
    {}

    virtual ~PixelBuffer() = 0;

    // Sub-classes MUST implement the following
    // Set a single pixel value at the specified location
    virtual bool setPixel(GRCOORD x, GRCOORD y, const PixRGBA pix) = 0;
    
    // Retrieve a single pixel value from the specified location 
    virtual PixRGBA getPixel(GRCOORD x, GRCOORD y) const = 0;
    
    // Copy the span of pixels into the pixel buffer
    virtual bool setSpan(GRCOORD x, GRCOORD y, const GRSIZE width, const PixRGBA * pix) = 0;
    
    // Set all pixels within the pixel buffer to the specified value
    virtual bool setAllPixels(const PixRGBA pix) = 0;

    GRSIZE getWidth() const { return this->width;}
    GRSIZE getHeight() const { return this->height;}

private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    PixelBuffer();

    // The width and height are const because
    // you don't want them to ever change
    const GRSIZE width;   //how many pixels wide
    const GRSIZE height;  // how many pixels tall

};