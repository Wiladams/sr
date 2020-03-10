#include "p5.hpp"
#include <stdio.h>

/*
https://social.msdn.microsoft.com/forums/vstudio/en-US/f7dacb67-485b-4bb7-b2ff-76c48adce0d6/webcam-vfwh-problem-black-screen
*/
HWND hWndC = nullptr;
HWND hwndParent = nullptr;
int nID = 0;

void setup()
{
    createCanvas(800, 600);


    //hWndC = capCreateCaptureWindowA ( "My Own Capture Window", 
    //    WS_VISIBLE , 0, 0, 640, 480, hwndParent, nID);
    //SendMessageA (hWndC, WM_CAP_DRIVER_CONNECT, 0 , 0L);
    //SendMessageA (hWndC, WM_CAP_SEQUENCE, 0, 0L);
    //SendMessageA (hWndC, WM_CAP_SEQUENCE_NOFILE, 0, 0L);

    hwndParent = gAppWindow->getHandle();
    hWndC = capCreateCaptureWindowA ( "My Own Capture Window", 
        WS_CHILD | WS_VISIBLE , 0, 0, 320, 240, hwndParent, nID);
    //hWndC = capCreateCaptureWindowA ( "My Own Capture Window", 
    //    WS_VISIBLE , 0, 0, 320, 240, hwndParent, nID);




    int res = capDriverConnect(hWndC, 0);
    printf("capdriverConnect: %d\n", res);

    //capCaptureSequenceNoFile(hWndC);
    
    res = capDlgVideoFormat(hWndC); 
    printf("capDlgVideoFormat: %d\n", res);

    capPreviewRate(hWndC, 33);  // 33 == ~30 fps
    capPreviewScale(hWndC,1);
    capPreview(hWndC, 1);
}