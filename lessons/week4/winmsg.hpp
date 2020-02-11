#pragma once

#include "w32.hpp"

// Basic type to encapsulate a mouse event
enum {
    KEYPRESSED,
    KEYRELEASED,
    KEYTYPED
};

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

struct KeyEvent {
    int activity;
    int keyCode;        // wparam
    int repeatCount;    // 0 - 15
    int scanCode;       // 16 - 23
    bool isExtended;    // 24
    bool wasDown;       // 30
};

// Some globals to set event handlers
typedef LRESULT (*WinMSGObserver)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef void (* KeyEventHandler)(const KeyEvent &e);
typedef void (* MouseEventHandler)(const MouseEvent &e);

static WinMSGObserver gPaintHandler = nullptr;

static WinMSGObserver gKeyboardHandler = nullptr;
static KeyEventHandler gKeyPressedHandler = nullptr;
static KeyEventHandler gKeyReleasedHandler = nullptr;
static KeyEventHandler gKeyTypedHandler = nullptr;


static WinMSGObserver gMouseHandler = nullptr;
static MouseEventHandler gMouseMovedHandler = nullptr;
static MouseEventHandler gMouseClickedHandler = nullptr;
static MouseEventHandler gMousePressedHandler = nullptr;
static MouseEventHandler gMouseReleasedHandler = nullptr;
static MouseEventHandler gMouseWheelHandler = nullptr;
static MouseEventHandler gMouseDraggedHandler = nullptr;

static WinMSGObserver gTouchHandler = nullptr;

static int keyCode = 0;
static int keyChar = 0;

LRESULT HandleKeyboardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;
    KeyEvent e;
    e.keyCode = (int)wParam;
    e.repeatCount =LOWORD(lParam);  // 0 - 15
    e.scanCode = ((lParam & 0xff0000) >> 16);        // 16 - 23
    e.isExtended = (lParam & 0x1000000) != 0;    // 24
    e.wasDown = (lParam & 0x40000000) != 0;    // 30

//printf("HandleKeyboardEvent: %04x\n", msg);

    switch(msg) {

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            e.activity = KEYPRESSED;
            keyCode = e.keyCode;
            if (gKeyPressedHandler) {
                gKeyPressedHandler(e);
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            e.activity = KEYRELEASED;
            keyCode = e.keyCode;
            if (gKeyReleasedHandler) {
                gKeyReleasedHandler(e);
            }
            break;
        case WM_CHAR:
        case WM_SYSCHAR:
            keyChar = (int)wParam;
            e.activity = KEYTYPED;

            if (gKeyTypedHandler) {
                gKeyTypedHandler(e);
            }
            break;
    }

    return res;
}

static bool mouseIsPressed = false;
static int mouseX = 0;
static int mouseY = 0;
static int pmouseX = 0;
static int pmouseY = 0;

LRESULT HandleMouseEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;
    
    // assign previous mouse position
    pmouseX = mouseX;
    pmouseY = mouseY;

    // assign new mouse position
    mouseX = GET_X_LPARAM(lParam);
    mouseY = GET_Y_LPARAM(lParam);

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
        case WM_LBUTTONDBLCLK:
	    case WM_MBUTTONDBLCLK:
	    case WM_RBUTTONDBLCLK:
            break;

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

LRESULT HandleTouchEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;

    // cInputs could be set to a maximum value (10) and
    // we could reuse the same allocated array each time
    // rather than allocating a new one each time.
    //print("wm_touch_event 0.0: ", wparam)
    int cInputs = LOWORD(wParam);
        
        //print("wm_touch_event 1.0: ", cInputs)
        
        TOUCHINPUT *pInputs = {new TOUCHINPUT[cInputs]{}};
        int cbSize = sizeof("TOUCHINPUT");

        //print("wm_touch_event 2.0: ", pInputs, cbSize)
        BOOL bResult = GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs,cbSize);
        //print("wm_touch_event 3.0: ", bResult)

        if (bResult == 0) {
            return 0; // C.GetLastError()
        }
        //print("wm_touch_event 4.0: ", bResult)
/*
        -- Construct an event with all the given information
        local events = {}

        POINT PT;
        for (int i=0; i<cInputs; i++) {
            PT.x = pInputs[i].x/100;
            PT.y = pInputs[i].y/100;
            //print("wm_touch_event 4.1: ", PT.x, PT.y)
            local bResult = C.ScreenToClient(hwnd, PT)
            --print("wm_touch_event 4.2: ", bResult, PT.x, PT.y)
            local event = {
                ID = pInputs[i].dwID;
                x = PT.x;
                y = PT.y;
                rawX = pInputs[i].x;
                rawY = pInputs[i].y;
            }

            if band(pInputs[i].dwMask, C.TOUCHINPUTMASKF_CONTACTAREA) ~= 0 then
                event.rawWidth = pInputs[i].cxContact;
                event.rawHeight = pInputs[i].cyContact;
                event.width = event.rawWidth/100;
                event.height = event.rawHeight/100;
            end

            table.insert(events, event)
        }
        //print("wm_touch_event 5.0: ", bResult)

        return events
*/
    return res;
}

#ifdef __cplusplus
extern "C" {
#endif


// These should be implemented by a module to be loaded
WIN_EXPORT void draw();
WIN_EXPORT void loop();
WIN_EXPORT void setup();


// IO Event Handlers
WIN_EXPORT LRESULT keyboardHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WIN_EXPORT void keyPressed(const KeyEvent &e);
WIN_EXPORT void keyReleased(const KeyEvent &e);
WIN_EXPORT void keyTyped(const KeyEvent &e);

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

    // Setup the routines that will handle
    // keyboard and mouse events
    void setupEventHandlers()
    {
        // we're going to look within our own module
        // to find handler functions
        HMODULE hInst = GetModuleHandleA(NULL);

        // Start with our default handlers
        gKeyboardHandler = HandleKeyboardEvent;
        gMouseHandler = HandleMouseEvent;
        gTouchHandler = HandleTouchEvent;


        // The user can specify their own handlers for io and
        // painting
        WinMSGObserver handler = (WinMSGObserver)GetProcAddress(hInst, "paintHandler");
        if (handler != nullptr) {
            gPaintHandler = handler;
        }


        handler = (WinMSGObserver)GetProcAddress(hInst, "keyboardHandler");
        if (handler != nullptr) {
                    printf("key handler: %p\n", handler);
            gKeyboardHandler = handler;
        }

        handler = (WinMSGObserver)GetProcAddress(hInst, "mouseHandler");
        if (handler != nullptr) {
            gMouseHandler = handler;
        }

        handler = (WinMSGObserver)GetProcAddress(hInst, "touchHandler");
        if (handler != nullptr) {
            gTouchHandler = handler;
        }

        //printf("mouseHandler: %p\n", gMouseHandler);
        // If the user implements various event handlers, they will 
        // be called automatically
        gMouseMovedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseMoved");
        gMouseClickedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseClicked");
        gMousePressedHandler = (MouseEventHandler)GetProcAddress(hInst, "mousePressed");
        gMouseReleasedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseReleased");
        gMouseWheelHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseWheel");
        gMouseDraggedHandler = (MouseEventHandler)GetProcAddress(hInst, "mouseDragged");

        // Keyboard event handling
        gKeyPressedHandler = (KeyEventHandler)GetProcAddress(hInst, "keyPressed");
        gKeyReleasedHandler = (KeyEventHandler)GetProcAddress(hInst, "keyReleased");
        gKeyTypedHandler = (KeyEventHandler)GetProcAddress(hInst, "keyTyped");

        // Touch event handling
    }


LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //printf("MSG: 0x%04x\n", msg);
    LRESULT res = 0;
	PAINTSTRUCT ps;
	HDC hdc;


    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    } else if ((msg >= WM_MOUSEFIRST) && (msg <= WM_MOUSELAST)) {
		if (gMouseHandler != nullptr) {
			return gMouseHandler(hWnd, msg, wParam, lParam);
		}
    } else if ((msg >= WM_KEYFIRST) && (msg <= WM_KEYLAST)) {
        if (gKeyboardHandler != nullptr) {
            gKeyboardHandler(hWnd, msg, wParam, lParam);
        }
    } else if (msg == WM_TOUCH) {
        if (gTouchHandler != nullptr) {
            gTouchHandler(hWnd, msg, wParam, lParam);
        }
            //res = TouchActivity(hwnd, msg, wparam, lparam)
    } else if (msg == WM_PAINT) {
        if (gPaintHandler != nullptr) {
            gPaintHandler(hWnd, msg, wParam, lParam);
        } else
        {
            /* code */
            res = DefWindowProcA(hWnd, msg, wParam, lParam);
        }
    } else {
        res = DefWindowProcA(hWnd, msg, wParam, lParam);
    }

    return res;
}


// A basic Windows event loop
void run()
{
    // Make sure we have all the event handlers connected

    setupEventHandlers();
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
            if (msg.message == WM_QUIT) {
                break;
            }

            res = TranslateMessage(&msg);
            res = DispatchMessageA(&msg);
        }
    }
}