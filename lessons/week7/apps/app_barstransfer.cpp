#include "p5.hpp"

int barSize = 200;

/*
    Change transparency based on position on the screen
    Lower right, most opaque
    upper left, most transparent

    This pixel transfer operation is a compound operation
    First, it determines an alpha value based on the screen
    location.  Then it applies that to the source pixel
    and last, it uses the regular SRCOVER transfer function 
    to actually blend that pixel with the destination.
*/
class TransferTrans : public PixelTransferOp
{
    // Create an instance of a SRCOVER transfer op
    // for later usage.
    PixelSRCOVER SRCOVER;

public:
    // This operator is called for every pixel that is to 
    // be rendered
    virtual PixRGBA operator()(GRCOORD x, GRCOORD y, const PixRGBA &src, const PixRGBA &dst) const
    {
        // Copy the source pixel
        PixRGBA pix = src;

        // Figure out the desired alpha value based
        // on the horizontal and vertical position
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

    noStroke();

    // horizontal green bar
    fill(colors.green);
    int y = constrain(mouseY, 0, height-barSize);
    rect(0,y, width-1,barSize);


    // vertical blue bar
    fill(colors.blue);
    int x = constrain(mouseX, 0, width-barSize);
    rect(x,0,barSize,height);
}

void setup()
{
    createCanvas(400,400);

    // call pixelOp() to install our
    // custom pixel transfer operation
    pixelOp(new TransferTrans());
}