
#include "NativeWindow.hpp"
#include <stdio.h>

void printEvent(const char *title, const MouseEvent &e)
{
    printf("%s: %d  At: %d %d\n", title, e.activity, e.x, e.y);
}


void mouseMoved(const MouseEvent &e)
{
    printEvent("MOUSEMOVED", e);
}


void mousePressed(const MouseEvent &e)
{
    printEvent("MOUSEPRESSED", e);
}

void mouseReleased(const MouseEvent &e)
{
    printEvent("MOUSERELEASED", e);
}

void mouseWheel(const MouseEvent &e)
{
    printEvent("MOUSEWHEEL", e);
}

void mouseClicked(const MouseEvent &e)
{
    printEvent("MOUSECLICKED", e);
}

void mouseDragged(const MouseEvent &e)
{
    printEvent("MOUSEDRAGGED", e);
}

void keyPressed(const KeyEvent &e)
{
    printf("KEYPRESSED\n");
}

void keyReleased(const KeyEvent &e)
{
    printf("KEYRELEASED\n");
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
