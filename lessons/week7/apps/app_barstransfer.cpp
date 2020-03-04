#include "p5.hpp"

int barSize = 200;

/*
    Change transparency based on position on the screen
    Lower right, most opaque
    upper left, most transparent
*/
class TransferTrans : public PixelTransferOp
{
    PixelSRCOVER SRCOVER;

public:
    virtual PixRGBA operator()(GRCOORD x, GRCOORD y, const PixRGBA &src, const PixRGBA &dst) const
    {
        PixRGBA pix = src;
        int xcontrib = MAP(mouseX,0,width,0,127);
        int ycontrib = MAP(mouseY,0,height,0,127);
        pix.alpha = xcontrib+ycontrib;

        return SRCOVER(x,y,pix,dst);
    }

};

void draw()
{   
    // baseline red
    background(colors.red);


    // horizontal green bar
    fill(colors.green);
    noStroke();
    int y = constrain(mouseY, 0, height-barSize);
    rect(0,y, width-1,barSize);


    // vertical blue bar
    noStroke();
    fill(colors.blue);
    int x = constrain(mouseX, 0, width-barSize);
    rect(x,0,barSize,height);

}

void setup()
{
    createCanvas(400,400);
    pixelOp(new TransferTrans());
}