#include "p5.hpp"

int rectX = 0;
int fr = 30; //starting FPS
PixRGBA clr;

void setup() 
{
    createCanvas(100,100);
    background(200);
    frameRate(fr); // Attempt to refresh at starting FPS
    clr = color(255, 0, 0);
}

void draw() {
  background(200);
  rectX += 1; // Move Rectangle

  if (rectX >= width) {
   // If you go off screen.
    if (fr == 30) {
      clr = color(0, 0, 255);
      fr = 10;
      frameRate(fr); // make frameRate 10 FPS
    } else {
      clr = color(255, 0, 0);
      fr = 30;
      frameRate(fr); // make frameRate 30 FPS
    }
    rectX = 0;
  }
  fill(clr);
  rect(rectX, 40, 20, 20);
}
