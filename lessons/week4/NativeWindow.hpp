#pragma once

#include "w32.hpp"
#include <stdio.h>

ATOM RegisterWindowClass(const char *wndclassname, WNDPROC msgproc, int style)
{
	HMODULE hInst = GetModuleHandleA(NULL);

	WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEXA);
    wcex.style          = style;
    wcex.lpfnWndProc    = msgproc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInst;
    wcex.hIcon          = NULL;		// LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = NULL;		// LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;      // ffi.cast("HBRUSH", C.COLOR_WINDOW+1);;		-- (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;		// NULL;
    wcex.lpszClassName  = wndclassname;
    wcex.hIconSm        = NULL;		// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	ATOM classAtom = RegisterClassExA(&wcex);

	return classAtom;
}



LRESULT MsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //printf("MSG: 0x%04x\n", msg);
    LRESULT res = 0;

    switch(msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        break;

        default:
            res = DefWindowProcA(hwnd, msg, wParam, lParam);
        break;
    }

    return res;
}




typedef void(*WinMSGObserver)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef void(*WinEventHandler)();

static WinMSGObserver gKeyboardHandler = nullptr;
static WinMSGObserver gMouseHandler = nullptr;

static WinEventHandler gMouseMovedHandler = nullptr;

class Window {
public:
    ATOM fwinclass;
    HWND fHandle;


    static void setupEventHandlers()
    {
        // we're going to look within our own module
        // to find handler functions
        HMODULE hInst = GetModuleHandleA(NULL);

        gMouseHandler = (WinMSGObserver)GetProcAddress(hInst, "mouseHandler");
        gMouseMovedHandler = (WinEventHandler)GetProcAddress(hInst, "mouseMoved");


        
        //printf("movedMouse: %p\n", hMouseMoved);
    }

    Window(const char *title, int width, int height)
    {
        static char * winclassname = "w32WindowClass";
        int winxstyle = 0;

        fwinclass = RegisterWindowClass(winclassname, MsgHandler, CS_HREDRAW | CS_VREDRAW);

	    HMODULE hInst = GetModuleHandleA(NULL);
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

        setupEventHandlers();
    }

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

    virtual void run()
    {
        // Do a typical Windows message pump
        MSG msg;
        BOOL res;

        while (true) {
            //print("LOOP")
            // we use peekmessage, so we don't stall on a GetMessage
            //while (C.PeekMessageA(msg, nil, 0, 0, C.PM_REMOVE) ~= 0) do
            BOOL haveMessage = PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0;
            if (haveMessage) {
                // If we see a quit message, it's time to stop the program
                // ideally we'd call an 'onQuit' and wait for that to return
                // before actually halting.  That will give the app a chance
                // to do some cleanup
                if (msg.message == WM_QUIT) {
                    printf("msgLoop - QUIT");
                    break;
                }

                res = TranslateMessage(&msg);
                res = DispatchMessageA(&msg);
            }
        }
    }


};


// We need this export so when the user defines their functions
// they will be listed as exports, and then at runtime we can 
// load their pointers from the module
#define WIN_EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

// These should be implemented by a module to be loaded
WIN_EXPORT void draw();
WIN_EXPORT void loop();
WIN_EXPORT void setup();


// IO Event Handlers
WIN_EXPORT void keyPressed();
WIN_EXPORT void keyReleased();
WIN_EXPORT void keyTyped();

WIN_EXPORT void mouseClicked();
WIN_EXPORT void mouseDragged();
WIN_EXPORT void mouseMoved();
WIN_EXPORT void mousePressed();
WIN_EXPORT void mouseReleased();
WIN_EXPORT void mouseWheel();

#ifdef __cplusplus
}
#endif

