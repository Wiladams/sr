#include "PixelBuffer.hpp"
#include "colors.hpp"
#include <math.h>


typedef void(* EllipseHandler)(PixelBuffer &pb, GRCOORD cx, GRCOORD cy, GRCOORD x, GRCOORD y, const PixRGBA color);

void Plot4EllipsePoints(PixelBuffer &pb, GRCOORD cx, GRCOORD cy, GRCOORD x, GRCOORD y, const PixRGBA color)
{
    pb.setPixel(cx + x, cy + y, color);
    pb.setPixel(cx - x, cy + y, color);
    pb.setPixel(cx - x, cy - y, color);
    pb.setPixel(cx + x, cy - y, color);
}

void fill2EllipseLines(PixelBuffer &pb, GRCOORD cx, GRCOORD cy, GRCOORD x, GRCOORD y, const PixRGBA color)
{
	int x1 = cx - x;
	int y1 = cy+y;
	int x2 = cx + x;
	int y2 = cy+y;

	//if (clipLine(pb->frame, x1, y1, x2, y2)) {
        pb.setPixels(x1, y1, x2-x1, color);
	//}
	
	y1 = cy - y;
	y2 = cy - y;
	//if (clipLine(pb->frame, x1, y1, x2, y2)) {
		pb.setPixels(x1, y2, x2 - x1, color);
	//}
}


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


    // drawPolyLine(int nPts, Point2D *pts)
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

    /*
        Ellipse drawing
    */
    void raster_rgba_ellipse(GRCOORD cx, GRCOORD cy, size_t xradius, size_t yradius, const PixRGBA color, EllipseHandler handler)
    {
        int twoasquare = 2 * xradius*xradius;
        int twobsquare = 2 * yradius*yradius;

	    int x = xradius;
	    int y = 0;

        int xchange = yradius*yradius*(1 - 2 * xradius);
        int ychange = xradius*xradius;
        int ellipseerror = 0;
        int stoppingx = twobsquare*xradius;
        int stoppingy = 0;

        // first set of points, sides
        while (stoppingx >= stoppingy)
        {
            handler(pb, cx, cy, x, y, color);
            y = y + 1;
            stoppingy = stoppingy + twoasquare;
            ellipseerror += ychange;
            ychange += twoasquare;

            if ((2 * ellipseerror + xchange) > 0) {
                x--;
                stoppingx -= twobsquare;
                ellipseerror += xchange;
                xchange += twobsquare;
            }
        }

        // second set of points, top and bottom
        x = 0;
        y = yradius;
        xchange = yradius*yradius;
        ychange = xradius*xradius*(1 - 2 * yradius);
        ellipseerror = 0;
        stoppingx = 0;
        stoppingy = twoasquare*yradius;

        while (stoppingx <= stoppingy) {
            handler(pb, cx, cy, x, y, color);
            x = x + 1;
            stoppingx = stoppingx + twobsquare;
            ellipseerror = ellipseerror + xchange;
            xchange = xchange + twobsquare;
            
            if ((2 * ellipseerror + ychange) > 0) {
                y = y - 1;
                stoppingy -= twoasquare;
                ellipseerror += ychange;
                ychange += twoasquare;
            }
        }
    }

    // strokeEllipse()
    bool strokeEllipse(GRCOORD cx, GRCOORD cy, size_t xradius, size_t yradius)
    {
        raster_rgba_ellipse(cx, cy, xradius, yradius, strokePix, Plot4EllipsePoints);
        
        return true;
    }

    // fillEllipse()
    bool fillEllipse(GRCOORD cx, GRCOORD cy, size_t xradius, size_t yradius)
    {
        raster_rgba_ellipse(cx, cy, xradius, yradius, fillPix, fill2EllipseLines);
        return false;
    }
    
    bool drawEllipse(GRCOORD cx, GRCOORD cy, size_t xradius, size_t yradius)
    {
        fillEllipse(cx, cy, xradius, yradius);
        strokeEllipse(cx, cy, xradius, yradius);

        return true;
    }

    // drawEllipse()



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