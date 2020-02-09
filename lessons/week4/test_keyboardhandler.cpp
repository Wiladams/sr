
#include "NativeWindow.hpp"
#include <stdio.h>

void printEvent(const char *title, const KeyEvent &e)
{
    printf("%s: %d  Code: %d\n", title, e.activity, e.keyCode);
}


void keyPressed(const KeyEvent &e)
{
    printEvent("KEYPRESSED", e);
}

void keyReleased(const KeyEvent &e)
{
    printEvent("KEYRELEASED", e);
}


void keyTyped(const KeyEvent &e)
{
    printf("KEYTYPED: %c\n", e.keyCode);
}

// Easiest test case, just show a window
// closing the window should terminate the app
void main()
{
    Window w("Window Title", 640, 480);
    w.show();
    w.run();
}
