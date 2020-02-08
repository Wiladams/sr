#pragma once

/*
    This file, and the Window class represent the connection to the Win32 
    User32 interface library.  The idea is, if you include this single header
    in your application .cpp file, you have everything you need to create 
    a window of a given size, do drawing, keyboard and mouse handling.

    No need for any Windows specific headers or any other libraries.  This
    single file (and it pulls in w32.hpp) contains everything you need.
*/

#include "w32.hpp"
#include <stdio.h>



// Basic type to encapsulate a mouse event
enum {
    MOUSEMOVED,
    MOUSEPRESSED,
    MOUSERELEASED,
    MOUSECLICKED,
    MOUSEDRAGGED,
    MOUSEWHEEL,
    MOUSEENTERED,
    MOUSELEFT
};

struct MouseEvent {
    int activity;
    int x;
    int y;
    bool control;
    bool shift;
    bool lbutton;
    bool rbutton;
    bool mbutton;
    bool xbutton1;
    bool xbutton2;
};

// Some globals to set event handlers
typedef LRESULT (*WinMSGObserver)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef void (* WinEventHandler)();
typedef void (* MouseEventHandler)(const MouseEvent &e);

static WinMSGObserver gKeyboardHandler = nullptr;
static WinEventHandler gKeyPressedHandler = nullptr;
static WinEventHandler gKeyReleasedHandler = nullptr;
static WinEventHandler gKeyTypedHandler = nullptr;


static WinMSGObserver gMouseHandler = nullptr;
static MouseEventHandler gMouseMovedHandler = nullptr;
static MouseEventHandler gMouseClickedHandler = nullptr;
static MouseEventHandler gMousePressedHandler = nullptr;
static MouseEventHandler gMouseReleasedHandler = nullptr;
static MouseEventHandler gMouseWheelHandler = nullptr;
static MouseEventHandler gMouseDraggedHandler = nullptr;

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
WIN_EXPORT LRESULT keyboardHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WIN_EXPORT void keyPressed();
WIN_EXPORT void keyReleased();
WIN_EXPORT void keyTyped();

WIN_EXPORT LRESULT mouseHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WIN_EXPORT void mouseClicked(const MouseEvent &e);
WIN_EXPORT void mouseDragged(const MouseEvent &e);
WIN_EXPORT void mouseMoved(const MouseEvent &e);
WIN_EXPORT void mousePressed(const MouseEvent &e);
WIN_EXPORT void mouseReleased(const MouseEvent &e);
WIN_EXPORT void mouseWheel(const MouseEvent &e);

#ifdef __cplusplus
}
#endif

static bool mouseIsPressed = false;
static int mouseX = 0;
static int mouseY = 0;
static int pmouseX = 0;
static int pmouseY = 0;

LRESULT mouseHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;
    
    // assign previous mouse position
    pmouseX = mouseX;
    pmouseY = mouseY;

    // assign new mouse position
    mouseX = LOWORD(lParam);
    mouseY = HIWORD(lParam);

    //printf("mouseHandler: 0x%04x  %d %d\n", msg, mouseX, mouseY);

    MouseEvent e;
    e.x = mouseX;
    e.y = mouseY;
    e.control = (wParam & MK_CONTROL) != 0;
    e.shift = (wParam& MK_SHIFT) != 0;
    e.lbutton = (wParam & MK_LBUTTON) != 0;
    e.rbutton = (wParam & MK_RBUTTON) != 0;
    e.mbutton = (wParam & MK_MBUTTON) != 0;
    e.xbutton1 = (wParam & MK_XBUTTON1) != 0;
    e.xbutton2 = (wParam & MK_XBUTTON2) != 0;
    

    mouseIsPressed = e.lbutton || e.rbutton || e.mbutton;

    switch(msg) {
        case WM_MOUSEMOVE:
            e.activity = MOUSEMOVED;

            if (gMouseMovedHandler != nullptr) {
                gMouseMovedHandler(e);
            }
            break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
            e.activity = MOUSEPRESSED;

            if (gMousePressedHandler != nullptr) {
                gMousePressedHandler(e);
            }
            break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
            e.activity = MOUSERELEASED;

            // call mouseReleased()
            if (gMouseReleasedHandler != nullptr) {
                gMouseReleasedHandler(e);
            }
            if (gMouseClickedHandler != nullptr) {
                gMouseClickedHandler(e);
            }
            break;
        case WM_MOUSEWHEEL:
            e.activity = MOUSEWHEEL;

            if (gMouseWheelHandler != nullptr) {
                gMouseWheelHandler(e);
            }
            break;
        case WM_MOUSELEAVE:
        //print("WM_MOUSELEAVE")
        default:
            
        break;
    }

    return res;
}

LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //printf("MSG: 0x%04x\n", msg);
    LRESULT res = 0;
	PAINTSTRUCT ps;
	HDC hdc;

    switch(msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        break;

	    case WM_LBUTTONDBLCLK:
	    case WM_MBUTTONDBLCLK:
	    case WM_RBUTTONDBLCLK:
		    if (gMouseHandler != nullptr) {
			    return gMouseHandler(hWnd, msg, wParam, lParam);
		    }
		break;

	    case WM_MOUSEWHEEL:
            //printf("WM_MOUSEWHEEL\n");
		    if (gMouseHandler != nullptr) {
			    return gMouseHandler(hWnd, msg, wParam, lParam);
		    }
		break;

	    case WM_MOUSEMOVE:
	    case WM_LBUTTONDOWN:
	    case WM_MBUTTONDOWN:
	    case WM_RBUTTONDOWN:
	    case WM_LBUTTONUP:
	    case WM_MBUTTONUP:
	    case WM_RBUTTONUP:
		    if (gMouseHandler != nullptr) {
			    return gMouseHandler(hWnd, msg, wParam, lParam);
		    }
		break;
	    
        case WM_PAINT:
		    hdc = BeginPaint(hWnd, &ps);

		    // bitblt bmhandle to client area
		    // we should actually look at the paint struct
		    // and only blit the part that needs to be drawn
            /*
		    if ((NULL != dcBuffer) && (nullptr != gPixelData)) {
			    ret = ::BitBlt(hdc,
				    ps.rcPaint.left, ps.rcPaint.top,
				    ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
				    dcBuffer,
				    ps.rcPaint.left, ps.rcPaint.top,
				    SRCCOPY);
		    }
            */
		    EndPaint(hWnd, &ps);
		break;

        default:
            res = DefWindowProcA(hWnd, msg, wParam, lParam);
        break;
    }

    return res;
}


class Window {
public:
    ATOM fwinclass;
    HWND fHandle;


    // Setup the routines that will handle
    // keyboard and mouse events
    static void setupEventHandlers()
    {
        // we're going to look within our own module
        // to find handler functions
        HMODULE hInst = GetModuleHandleA(NULL);

        gMouseHandler = (WinMSGObserver)GetProcAddress(hInst, "mouseHandler");
        //printf("mouseHandler: %p\n", gMouseHandler);

        gMouseMovedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseMoved");
        gMouseClickedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseClicked");
        gMousePressedHandler = (MouseEventHandler)GetProcAddress(hInst, "mousePressed");
        gMouseReleasedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseReleased");
        gMouseWheelHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseWheel");
        gMouseDraggedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseDragged");

    }

    /*
        Basic constructor for a window class

        Must register a window class
        create window handle
        initialize event handling
    */
    Window(const char *title, int width, int height)
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

        setupEventHandlers();
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

    // A basic Windows event loop
    
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




