/*
    Test the guistyle object.

    The guistyle object draws the raised and sunken rectangles
    that are typical of UI elements such as buttons and sliders.
*/

#include "p5.hpp"
#include "guistyle.hpp"

GUIStyle gs;

void draw()
{
    background(colors.pink);

    gs.setBorderWidth(4);
    gs.drawSunkenRect(10, 10, 320, 40);
    gs.drawRaisedRect(10, 60, 320, 40);

    gs.setBorderWidth(6);
    gs.setBaseColor(colors.darkCyan);
    gs.drawSunkenRect(10, 120, 40, 320);
    gs.drawRaisedRect(60, 120, 40, 320);
    
    gs.setBorderWidth(2);
    gs.setBaseColor(colors.ltGray);
    gs.drawSunkenRect(320, 120, 40, 20);
    gs.drawRaisedRect(320, 160, 40, 20);

    noLoop();
}

void setup()
{
    createCanvas(640,480);
}