#include "p5.hpp"

    // helper for writing color to array
void writeColor(Image &img, size_t x, size_t y, int red, int green, int blue, int alpha) 
{
    //img.setPixel(x, y, color(red, green, blue, alpha));

        int index = (x + y * width) * 4;
        img.pixels[index] = blue;
        img.pixels[index + 1] = green;
        img.pixels[index + 2] = red;
        img.pixels[index + 3] = alpha;

}

void setup() 
{
    Image img(100, 100); // same as new p5.Image(100, 100);
    img.loadPixels();
    createCanvas(100, 100);
    background(0);

    size_t  x, y;
    // fill with random colors
    for (y = 0; y < img.height; y++) 
    {
        for (x = 0; x < img.width; x++) 
        {
            int red = random(255);
            int green = random(255);
            int blue = random(255);
            int alpha = 255;
            writeColor(img, x, y, red, green, blue, alpha);
        }
    }

    // draw a red line
    y = 0;
    for (x = 0; x < img.width; x++) {
        writeColor(img, x, y, 255, 0, 0, 255);
    }

    // draw a green line
    y = img.height - 1;
    for (x = 0; x < img.width; x++) {
        writeColor(img, x, y, 0, 255, 0, 255);
    }

    img.updatePixels();
    image(img, 0, 0);

}
