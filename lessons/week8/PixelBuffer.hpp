#pragma once

#include <stdint.h>
#include <stdio.h>

#include "grtypes.hpp"
#include "PixelOps.hpp"

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

    virtual ~PixelBuffer() = 0 {};

    // Sub-classes MUST implement the following
    // Set a single pixel value at the specified location
    virtual bool setPixel(GRCOORD x, GRCOORD y, const PixRGBA &pix) = 0;
    
    // Transfer a pixel using an operation
    virtual bool transferPixel(GRCOORD x, GRCOORD y, const PixRGBA &src, const PixelTransferOp &tOp)
    {
        PixRGBA dst = getPixel(x, y);
        PixRGBA pix = tOp(x, y, src, dst);

        return setPixel(x, y, pix);
    }

    // Retrieve a single pixel value from the specified location 
    virtual PixRGBA getPixel(GRCOORD x, GRCOORD y) const = 0;
    
    // Draw a horizontal line
    virtual void setPixels(GRCOORD x, GRCOORD y, GRSIZE width, const PixRGBA pix) {
        for (int col=x; col<x+width; col++) {
            setPixel(col,y, pix);
        }
    }

    // Copy the span of pixels into the pixel buffer
    virtual bool setSpan(GRCOORD x, GRCOORD y, const GRSIZE width, const PixRGBA * pix) = 0;
    
    // Set all pixels within the pixel buffer to the specified value
    virtual bool setAllPixels(const PixRGBA pix) = 0;

    virtual bool blit(const PixelBuffer &src, 
        int srcX, int srcY, int srcWidth, int srcHeight, 
        int destX, int destY, int destWidth, int destHeight)
    {
        //printf("srcWidth: %d\n", srcWidth);
        //printf("srcHeight: %d\n", srcHeight);

        for (int row=destY;row<(destY+destHeight-1);row++){
            for (int col=destX; col<(destX+destWidth-1); col++)
            {
                int dx = MAP(col, destX, destX+destWidth-1, srcX, srcX+srcWidth-1);
                int dy = MAP(row, destY, destY+destHeight-1, srcY, srcY+srcHeight-1);

                PixRGBA pix = src.getPixel(dx,dy);
                //printf("blit, pixel: %d %d (%d, %d, %d)\n", dx, dy, pix.red, pix.green, pix.blue);

                setPixel(col, row, pix);
            }
        }

        return true;
    }

    GRSIZE getWidth() const { return this->width;}
    GRSIZE getHeight() const { return this->height;}
    virtual const PixRGBA * getData() const = 0;

private:
    // private default constructor, so this can not
    // be an un-initialized element in an array 
    PixelBuffer();

    // The width and height are const because
    // you don't want them to ever change
    const GRSIZE width;   //how many pixels wide
    const GRSIZE height;  // how many pixels tall

};
