/*
    This app uses the VfwCameraDevice to display 
    the camera preview on the screen in its
    own window.
*/

#include "p5.hpp"

#include <stdio.h>

#include "VfwCameraDevice.hpp"
#include "PixelBufferRGB24.hpp"
#include "guistyle.hpp"

//VfwCameraDevice camera(640, 480);
VfwCameraDevice camera(0, 640, 480, 30, WS_VISIBLE, nullptr);
VfwDeviceDriver driver(0);


// This is here to do a blit upside down.
// by default vfw has the image bottom to top
void BLIT(PixelBuffer &src, const int x, const int y)
{
    int h = src.getHeight();

    for (int row=0; row < (src.getHeight()-1); row++)
    {
        for (int col=0; col<(src.getWidth()-1); col++)
        {
            gAppSurface->setPixel(x+col, y+(h-1)-row, src.getPixel(col,row));
        }
    }
}

/*
    When we grab a frame, we then display it
*/
LRESULT onFrameGrabbed(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    //printf("==== onFrameGrabbed ====\n");
    //printVideoHeader(lpVHdr);
    PixelBufferRGB24 pb(640, 480, lpVHdr->lpData);

    //PixelSRCOVER blendOp;
    PixelSRCCOPY blendOp;
    int srcX = 0;
    int srcY = 0;
    int srcWidth = 640;
    int srcHeight = 480;
    int x = 90;
    int y = 6;
    BLIT(pb, x, y);

    // Can't do it this way as it needs to be flipped vertically
    //gAppSurface->blit(pb, srcX, srcY, srcWidth, srcHeight, x, y, srcWidth, srcHeight);

    return 1;
}

/*
    The draw() function is called on the frameRate timer.  The only thing
    we want to do here in this particular application is tell the 
    camera to grabSingleFrame().

    We leave it to the callback function to actually do the drawing of
    that grabbed frame.

    If we want to do something interesting with drawing the frame here,
    we need to capture the frame into the camera, and ask the camera
    for it.
*/
void draw()
{
    // Draw sunken rectangle on the left for tools
    GUIStyle gs(colors.ltGray, 2);

    // sunken rect for tool area
    gs.drawSunkenRect(4,4, 80, height-8);

    // sunken rect for live image
    gs.drawSunkenRect(88,4, 648, height-8);

    camera.grabSingleFrame();
}

void setupCamera()
{
    printf("Driver: [%d - %d] %s\n", driver.getIndex(), driver.isValid(), driver.getName());

    if (!camera.connectToDriver(driver)) {
        printf("failed to connect to driver\n");
        return ;
    }

    camera.selectVideoSource();
    camera.selectVideoFormat();
    camera.setCallbackOnFrame(onFrameGrabbed);
    camera.hidePreview();
}

void setup()
{
    createCanvas(800, 600);
    setupCamera();
}