#pragma once

static const int toolWidth = 64;
static const int toolHeight = 64;

enum {
    LINE,
    RECTANGLE,
    ELLIPSE,
    FREEHAND
};

class ToolSelector {
    int originX;
    int originY;

public:
    ToolSelector(int x, int y)
        :originX(x), originY(y)
    {

    }

    void draw()
    {
        stroke(colors.black);
        fill(colors.white);
        rect(originX, originY, toolWidth,toolHeight*6);

        for (int i=1; i<6; i++){
            line(originX, originY+i*toolHeight, originX+toolWidth, originY+i*toolHeight);
        }

        // Draw tool icons
        // line
        line(originX+12, originY+12, toolWidth-8,toolHeight-8);

        // Rectangle
        fill(0xC0);
        rect(originX+12, 1*toolHeight+originY+12, toolWidth-20, toolHeight-20);

        // Ellipse
        ellipse(originX+toolWidth/2, 2*toolHeight+originY+toolHeight/2, toolWidth/2-12, toolHeight/2-12);
    }    
};
