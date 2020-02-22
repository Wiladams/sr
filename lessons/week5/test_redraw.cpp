#include "p5.hpp"

int x = 0;
void setup() {
   createCanvas(100, 100);
   noLoop();
}

void draw() {
   background(204);
   line(x, 0, x, height);
}

void mousePressed(const MouseEvent &e) {
   x += 2;
   if (x > width-1)
    x = 0;
    
   redraw();
}