
#include <stdio.h>

#include "NativeWindow.hpp"
#include "winmsg.hpp"



void printEvent(const char *title, const KeyEvent &e)
{
    printf("%s: %d  Code: %d\n", title, e.activity, e.keyCode);
}


LRESULT onPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    printf("onPaint\n");
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void mouseClicked(const MouseEvent &e)
{
    forceDraw();
}

// Easiest test case, just show a window
// closing the window should terminate the app
void main()
{
    Window w("Window Title", 640, 480);
    w.show();
    run();
}