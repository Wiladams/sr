#pragma once
#pragma comment(lib, "user32.lib")

/*
    This file, and the Window class represent the connection to the Win32 
    User32 interface library.  The idea is, if you include this single header
    in your application .cpp file, you have everything you need to create 
    a window of a given size, do drawing, keyboard and mouse handling.

    No need for any Windows specific headers or any other libraries.  This
    single file (and it pulls in w32.hpp) contains everything you need.
*/


#include <stdio.h>

#include "w32.hpp"
#include "winmsg.hpp"
#include "PixelBufferRGBA32.hpp"

/*
int handlePaint()

		hdc = BeginPaint(hWnd, &ps);
        
        int xDest = 0;
        int yDest = 0;
        int pResult = StretchDIBits(hdc,
                xDest,yDest,
                imgSize.w,imgSize.h,
                0,0,
                imgSize.w, imgSize.h,
                surface.pixelData.data,surface.info,
                DIB_RGB_COLORS,SRCCOPY)
        
		EndPaint(hWnd, &ps);
*/

class Window {
public:
    ATOM fwinclass;
    HWND fHandle;
    PixelBuffer * fBackground;

    /*
        Basic constructor for a window class

        Must register a window class
        create window handle
        initialize event handling
    */
    Window(const char *title, int width, int height, WNDPROC handler = MsgHandler)
        : fBackground(nullptr)
    {
        HMODULE hInst = GetModuleHandleA(NULL);
        
        // Register the window class
        static char * winclassname = "w32WindowClass";
        WNDCLASSEXA wcex;
        wcex.cbSize = sizeof(WNDCLASSEXA);
        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = MsgHandler;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInst;
        wcex.hIcon          = NULL;		// LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));
        wcex.hCursor        = NULL;		// LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground  = NULL;      // ffi.cast("HBRUSH", C.COLOR_WINDOW+1);;		-- (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = NULL;		// NULL;
        wcex.lpszClassName  = winclassname;
        wcex.hIconSm        = NULL;		// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

        fwinclass = RegisterClassExA(&wcex);

        // Create the window handle
        int winxstyle = 0;
	    //HMODULE hInst = GetModuleHandleA(NULL);
	    fHandle = CreateWindowExA(
		    winxstyle,
		    winclassname,
		    title,
		    WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
		    NULL,	
		    NULL,	
		    hInst,
		    NULL);
    }

    // Virtual destructor so the window can be sub-classed
    virtual ~Window(){
        //printf("~Window()");
        DestroyWindow(fHandle);
    }



    // All the methods that are useful
    bool isValid() {return fHandle != NULL;}

    // Hide the window
    void hide()
    {
        ShowWindow(fHandle, SW_HIDE);
    }

    // Show the window
    void show()
    {
        ShowWindow(fHandle, SW_SHOWNORMAL);
    }

    void setBackground(PixelBuffer *pb)
    {
        fBackground = pb;
    }
};




