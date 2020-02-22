#include "p5.hpp"

int x = 0;
void setup() {
   createCanvas(300, 200);
   noLoop();
}

void draw() {
    background(204);
    x += 1;
    if (x > width-1)
        x = 0;

    line(x, 0, x, height);
}

void mousePressed(const MouseEvent &e) {
   redraw(5);
}