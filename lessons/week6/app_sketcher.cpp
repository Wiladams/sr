#include "p5.hpp"
#include "collections.hpp"
#include "checkerboard.hpp"
#include "ToolSelector.hpp"

static const int edgeMargin = 10;

static const int interMargin = 4;
static const int canvasWidth = 1024;
static const int canvasHeight =768;

PixRGBA  randomColor()
{
    int r = random(30,255);
    int g = random(30,255);
    int b = random(30,255);
    
    return PixRGBA(r,g,b,126);
}




class ColorSelector {
    int originX;
    int originY;
    Queue colorQ;
    Checkerboard cb;

public:
    ColorSelector(int x, int y)
        :originX(x), originY(y),
        cb(x, y, 320, 64, 8*10, 16)
    {
        // setup selection of colors
        // twenty colors including transparent
        colorQ.enqueue(new PixRGBA(0x00000000));
        colorQ.enqueue(new PixRGBA(0xff000000));
        colorQ.enqueue(new PixRGBA(0xffffffff));

        for (int i=1; i<26;i++) {
            int r = random(30,255);
            int g = random(30,255);
            int b = random(30,255);

            colorQ.enqueue(new PixRGBA(r,g,b,255));
        }
    }
    
    void draw()
    {


        cb.draw();
        
        stroke(colors.black);
        noFill();
        rect(originX, originY, 320, 64);

        // draw color rectangles
        stroke(colors.black);
        int row = 0;
        int column = 1;
        QueueIterator qi(colorQ);
        PixRGBA * aColor;
        while (qi.next((void **)&aColor))
        {
            //printf("pos: %d %d\n", row, column);
        
            fill(*aColor);
            rect(originX + (column-1)*32,originY+(row*32),
                32,32); 

            column = column+1;
            if (column % 15 == 0)
            {
                column = 1;
                row = row + 1;
            }
        }
    }

    void moveTo(int x, int y)
    {
        originX = x;
        originY = y;
    }
};



ToolSelector ts(edgeMargin, edgeMargin);
ColorSelector cs(edgeMargin + toolWidth + interMargin, canvasHeight-64-10);

void draw()
{
    background(0xC0);

    ts.draw();
    cs.draw();
}

void setup()
{
    createCanvas(canvasWidth, canvasHeight);
}