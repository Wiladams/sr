#include "p5.hpp"
#include "collections.hpp"

static const int edgeMargin = 10;
static const int toolWidth = 64;
static const int toolHeight = 64;
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


class ToolSelector {
public:
    void draw()
    {
        stroke(colors.black);
        fill(colors.white);
        rect(edgeMargin, edgeMargin, toolWidth,toolHeight*6);
    }    
};

class ColorSelector {
    int originX;
    int originY;
    Queue colorQ;

public:
    ColorSelector(int x, int y)
        :originX(x), originY(y)
    {
        // setup selection of colors
        // twenty colors including transparent
        colorQ.enqueue(new PixRGBA(0x00000000));
        colorQ.enqueue(new PixRGBA(0xff000000));
        colorQ.enqueue(new PixRGBA(0xffffffff));

        for (int i=1; i<18;i++) {
            int r = random(30,255);
            int g = random(30,255);
            int b = random(30,255);

            colorQ.enqueue(new PixRGBA(r,g,b,255));
        }
    }
    
    void draw()
    {
        stroke(colors.black);
        fill(colors.white);
        rect(originX, originY, 320, 64);

        // draw color rectangles
        int row = 0;
        int column = 0;
        QueueIterator qi(colorQ);
        PixRGBA * aColor;
        while (qi.next((void **)&aColor))
        {
            if (column % 11 == 0)
            {
                column = 0;
                row = row + 1;
            }
            printf("pos: %d %d\n", row, column);
        
            fill(*aColor);
            rect(originX + (column)*32,originY+(row*32),
                32,32); 

            column = column+1;  
        }
    }

    void moveTo(int x, int y)
    {
        originX = x;
        originY = y;
    }
};



ToolSelector ts;
ColorSelector cs(edgeMargin + toolWidth + interMargin, canvasHeight-64-10);

void draw()
{
    background(0xC0);

    ts.draw();
    cs.draw();
}

void setup()
{
    createCanvas(1024, 768);
}