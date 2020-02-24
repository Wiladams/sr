/*
    Draw a checkerboard pattern, assuming the p5 environment
*/

#include "p5.hpp"

struct Checkerboard 
{
    int originX;
    int originY;
    PixRGBA color1;
    PixRGBA color2;
    int tileWidth;
    int tileHeight;
    int boxWidth;
    int boxHeight;
    int rows;
    int columns;

    Checkerboard(int x, int y, int width, int height, size_t columns = 8, int rows = 8)
        : originX(x), originY(y), color1(color(30)), color2(color(127))
    {
        this->rows = rows;
        this->columns = columns;

        tileWidth = width / columns;
	    tileHeight = height / rows;
	    boxWidth = tileWidth / 2;
	    boxHeight = tileHeight / 2;
    }

    void draw()
    {
        //rectMode(CORNER);
	    noStroke();
    
        PixRGBA c1 = color1;
        PixRGBA c2 = color2;

        for (int r = 0; r < rows-1; r++) {
        // Flip which color comes first
        // per each row
            if (r % 2 == 0) {
                c1 = color1;
                c2 = color2;
            } else {
                c1 = color2;
                c2 = color1;
            }
        

            for (int c = 0; c <  columns-1; c++) {
                // alternate colors per column
                if (c % 2 == 0) {
		            fill(c1);
                    rect(originX+c*tileWidth, originY+r*tileHeight, tileWidth, tileHeight);
                } else {
                    fill(c2);
                    rect(originX+c*tileWidth, originY+r*tileHeight, tileWidth, tileHeight);
                }
            }
        }
    }
};


