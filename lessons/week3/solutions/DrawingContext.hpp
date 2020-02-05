#include "FrameBuffer.hpp"
#include "colors.hpp"
#include <math.h>

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

    /*
    strokeLine()

    Stroke a line using the current stroking pixel.
    Uses Bresenham line drawing.  Does not check for clipping.

    Note: an easy optimization would be to use the specialized
    horizontal and vertical line drawing routines when necessary
    */
    bool strokeLine(GRCOORD x1, GRCOORD y1, GRCOORD x2, GRCOORD y2)
    {
	    int sdx, sdy, dxabs, dyabs;
	    unsigned int x, y, px, py;

        int dx = x2 - x1;      // the horizontal distance of the line
        int dy = y2 - y1;      // the vertical distance of the line
        dxabs = abs(dx);
        dyabs = abs(dy);
        sdx = sgn(dx);
        sdy = sgn(dy);
        x = dyabs >> 1;
        y = dxabs >> 1;
        px = x1;
        py = y1;

        this->fb.setPixel(x1, y1, this->strokePix);

        if (dxabs >= dyabs) // the line is more horizontal than vertical
        {
            for (size_t i = 0; i<dxabs; i++)
            {
                y = y + dyabs;
                if (y >= (unsigned int)dxabs)
                {
                    y = y - dxabs;
                    py = py + sdy;
                }
                px = px + sdx;
                this->fb.setPixel(px, py, this->strokePix);
            }
        } else // the line is more vertical than horizontal
        {
            for (size_t i = 0; i<dyabs; i++)
            {
                x += dxabs;
                if (x >= (unsigned int)dyabs)
                {
                    x -= dyabs;
                    px += sdx;
                }
                py += sdy;
                this->fb.setPixel(px, py, this->strokePix);
            }
        }

        return true;
    }


    // drawPolyLine()
    // drawCubicBezier()



    bool strokeRectangle(GRCOORD x, GRCOORD y, GRSIZE width, GRSIZE height)
    {
        // Draw Horizontal Lines
        strokeHorizontalLine(x, y ,width);
        strokeHorizontalLine(x, y+height-1, width);

        // Draw Vertical lines
        strokeVerticalLine(x,y,height);
        strokeVerticalLine(x+width-1, y, height);

        return true;
    }

    bool fillRectangle(GRCOORD x, GRCOORD y, GRSIZE width, GRSIZE height)
    {
        // Fill up a single row of pixels
        for (GRSIZE idx=0;idx < width;idx++)
        {
            this->scratch[idx] = this->fillPix;
        }

        // use that scratch line to fill all lines of the rectangle
        for (GRSIZE idx = 0; idx<height; idx++)
        {
            this->fb.setPixels(x, y+idx, width, this->scratch);
        }

        return true;
    }
    
    bool drawRectangle(GRCOORD x, GRCOORD y, GRSIZE width, GRSIZE height)
    {
        // fill rectangle
        fillRectangle(x, y, width, height);

        // stroke rectangle
        strokeRectangle(x, y, width, height);
        
        return true;
    }

    // drawRoundedRectangle()
    // strokeRoundedRectangle()
    // fillRoundedRectangle()

    // drawEllipse()
    // strokeEllipse()
    // fillEllipse()


    // strokeTriangle()
    bool strokeTriangle(const GRTriangle &geo)
    {
        strokeLine(geo.x1, geo.y1, geo.x2, geo.y2);
        strokeLine(geo.x2, geo.y2, geo.x3, geo.y3);
        strokeLine(geo.x3, geo.y3, geo.x1, geo.y1);

        return true;
    }

    // fillTriangle()
    bool fillTriangle(const GRTriangle &geo)
    {
        return false;
    }

    // drawTriangle()
    bool drawTriangle(const GRTriangle geo)
    {
        fillTriangle(geo);
        strokeTriangle(geo);

        return true;
    }
    
    // drawPolygon()
    // strokePolygon()
    // fillPolygon()



};