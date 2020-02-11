
#include "NativeWindow.hpp"
#include <stdio.h>


// Easiest test case, just show a window
// closing the window should terminate the app
void main()
{
    Window w("Window Title", 640, 480);
    w.show();
    run();
}