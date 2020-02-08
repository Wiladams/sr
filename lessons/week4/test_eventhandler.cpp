
#include "NativeWindow.hpp"
#include <stdio.h>

void mouseHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}

void mouseMoved()
{
    printf("MOUSE MOVED\n");
}

// Easiest test case, just show a window
// closing the window should terminate the app
void main()
{
    Window w("Window Title", 640, 480);
    w.show();
    w.run();
}
