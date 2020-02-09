#include "PixelBuffer.hpp"
#include "colors.hpp"
#include <math.h>

class DrawingContext {

private:
    PixelBuffer &pb;        // The pixel buffer we will be drawing into
    PixRGBA strokePix;      // pixel color for stroking
    PixRGBA fillPix;        // pixel color for filling
    PixRGBA bgPix;          // pixel color for background

    PixRGBA *scratch;

public:
    DrawingContext(PixelBuffer &pb)
    :pb(pb), 
    strokePix(colors.black), 
    fillPix(colors.white),
    bgPix(colors.gray50)
    {
        // create a scratch row for better optimization
        // of copy operators
        this->scratch = {new PixRGBA[pb.getWidth()]{}};
    }

    virtual ~DrawingContext()
    {
        delete [] scratch;
    }

    bool setBackground(const PixRGBA pix)
    {
        bgPix = pix;
        return true;
    }

    bool setFill(const PixRGBA pix)
    {
        fillPix = pix;
        
        // Fill up a single row of pixels
        for (GRSIZE idx=0;idx < pb.getWidth();idx++)
        {
            this->scratch[idx] = pix;
        }

        return true;
    }

    bool setStroke(const PixRGBA pix)
    {
        strokePix = pix;
        return true;
    }

    // clear the canvas to the background color
    bool clear()
    {
        pb.setAllPixels(bgPix);
        return true;
    }

    // Uses fill color
    // Should be able to apply a drawing operator
    bool fillPixel(GRCOORD x, GRCOORD y)
    {
        pb.setPixel(x, y, fillPix);
        return true;
    }

    bool strokeHorizontalLine(GRCOORD x, GRCOORD y, GRSIZE width)
    {
        GRCOORD idx = x;
        while (idx < x+width)
        {
            pb.setPixel(idx, y, strokePix);
            idx = idx + 1;
        }

        return true;
    }

    bool strokeVerticalLine(GRCOORD x, GRCOORD y, GRSIZE length)
    {
        GRCOORD idx = y;
        while (idx < y+length) {
            this->pb.setPixel(x, idx, this->strokePix);
            idx = idx + 1;
        }
        return true;
    }

    /*
    strokeLine()

    Stroke a line using the current stroking pixel.
    Uses Bresenham line drawing.  Does not check for clipping,
    but the underlying PixelBuffer might.

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

        this->pb.setPixel(x1, y1, this->strokePix);

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
                this->pb.setPixel(px, py, this->strokePix);
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
                this->pb.setPixel(px, py, this->strokePix);
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
            this->pb.setSpan(x, y+idx, width, this->scratch);
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
        strokeLine(geo.verts[0].x, geo.verts[0].y, geo.verts[1].x, geo.verts[1].y);
        strokeLine(geo.verts[1].x, geo.verts[1].y, geo.verts[2].x, geo.verts[2].y);
        strokeLine(geo.verts[2].x, geo.verts[2].y, geo.verts[0].x, geo.verts[0].y);

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