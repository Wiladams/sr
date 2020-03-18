/*
    This app uses the VfwCameraDevice to display 
    the camera preview on the screen in its
    own window.
*/

#include "p5.hpp"

#include <stdio.h>

#include "VfwCameraDevice.hpp"
#include "PixelBufferRGB24.hpp"

//VfwCameraDevice camera(640, 480);
VfwCameraDevice camera(0, 640, 480, 30, WS_VISIBLE, nullptr);
VfwDeviceDriver driver(0);


void printVideoHeader(LPVIDEOHDR lpVHdr)
{
    printf("Data: %p\n", lpVHdr->lpData);   
    printf("Buffer Length: %d\n", lpVHdr->dwBufferLength);
    printf("Bytes Used: %d\n", lpVHdr->dwBytesUsed);
    printf("Time Captured: %d\n", lpVHdr->dwTimeCaptured);    
    printf("dwUser: %p\n", (void *)lpVHdr->dwUser);    
    printf("dwFlags: %d\n", lpVHdr->dwFlags);    
}

// This is here to do a blit upside down.
// by default vfw has the image bottom to top
void BLIT(PixelBuffer &src)
{
    int h = src.getHeight();

    for (int row=0; row < (src.getHeight()-1); row++)
    {
        for (int col=0; col<(src.getWidth()-1); col++)
        {
            gAppSurface->setPixel(col, (h-1)-row, src.getPixel(col,row));
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
    int x = 0;
    int y = 0;
    BLIT(pb);
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

    
    //camera.setPreviewRate(33);
    //camera.setPreviewScale(1);
    camera.setCallbackOnFrame(onFrameGrabbed);
    camera.hidePreview();
    //camera.startPreview();
}

void setup()
{
    createCanvas(800, 600);
    setupCamera();
}