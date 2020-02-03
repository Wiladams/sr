#include "FrameBuffer.hpp"
#include "colors.hpp"

class DrawingContext {

private:
    FrameBuffer &fb;
    PixRGBA strokePix;
    PixRGBA fillPix;
    PixRGBA bgPix;

    PixRGBA *scratch;

public:
    DrawingContext(FrameBuffer &fb)
    :fb(fb), 
    strokePix(colors.black), 
    fillPix(colors.white),
    bgPix(colors.gray50)
    {
        // create a scratch row for better optimization
        // of copy operators
        this->scratch = {new PixRGBA[fb.getWidth()]{}};
    }

    virtual ~DrawingContext()
    {
        delete [] scratch;
    }

    bool setBackground(PixRGBA pix)
    {
        this->bgPix = pix;
        return true;
    }

    bool setFill(const PixRGBA pix)
    {
        this->fillPix = pix;
        
        // Fill up a single row of pixels
        for (GRSIZE idx=0;idx < this->fb.getWidth();idx++)
        {
            this->scratch[idx] = pix;
        }

        return true;
    }

    bool setStroke(const PixRGBA pix)
    {
        this->strokePix = pix;
        return true;
    }

    // clear the canvas to the background color
    bool clear()
    {
        this->fb.setAllPixels(this->bgPix);
        return true;
    }

    // Uses fill color
    // Should be able to apply a drawing operator
    bool fillPixel(GRCOORD x, GRCOORD y)
    {
        this->fb.setPixel(x, y, this->fillPix);
        return true;
    }

    bool strokeHorizontalLine(GRCOORD x, GRCOORD y, GRSIZE width)
    {
        GRCOORD idx = x;
        while (idx < x+width)
        {
            this->fb.setPixel(idx, y, this->strokePix);
            idx = idx + 1;
        }

        return true;
    }

    bool strokeVerticalLine(GRCOORD x, GRCOORD y, GRSIZE length)
    {
        GRCOORD idx = y;
        while (idx < y+length) {
            this->fb.setPixel(x, idx, this->strokePix);
            idx = idx + 1;
        }
        return true;
    }

    // drawLine(FBCOORD x1, FBCOORD y1, FBCOORD x2, FBCOORD y2)
    // drawPolyLine()
    // drawCubicBezier()

    // drawRectangle()
    // strokeRectangle()
    bool fillRectangle(GRCOORD x, GRCOORD y, GRSIZE width, GRSIZE height)
    {
        // Fill up a single row of pixels
        for (GRSIZE idx=0;idx < width;idx++)
        {
            this->scratch[idx] = this->fillPix;
        }

        for (GRSIZE idx = 0; idx<height; idx++)
        {
            this->fb.setPixels(x, y+idx, width, this->scratch);
        }

        return true;
    }

    // drawRoundedRectangle()
    // strokeRoundedRectangle()
    // fillRoundedRectangle()

    // drawEllipse()
    // strokeEllipse()
    // fillEllipse()

    // drawTriangle()
    // strokeTriangle()
    // fillTriangle()

    // drawPolygon()
    // strokePolygon()
    // fillPolygon()



};