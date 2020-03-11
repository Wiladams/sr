#include "p5.hpp"

#include "screensnapshot.hpp"

ScreenSnapshot *ss = nullptr;


void draw()
{
    ss->moveNext();
    
    // no scaling, just blit
    image(ss->getCurrent(), 0,0);
    
    // do this if you want to try scaling
    //gAppSurface->blit(ss->getCurrent(), 0, 0, ss->getCurrent().getWidth(), ss->getCurrent().getHeight(), 0, 0, width/2, height/2);

}

void setup()
{
    createCanvas(1280,1080);
    ss = new ScreenSnapshot(0, 0, 1280, 1080);
}