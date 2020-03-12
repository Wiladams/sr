/*
    Demonstration of some simple slide animations
*/

#include "p5.hpp"
#include "codec_targa.hpp"
#include "stopwatch.hpp"
#include "iparametric.hpp"

// A type to interpolate doubles
typedef ValueInterpolator<double> ParamD;

PixelBuffer *apb = nullptr;
PixelSRCOVER blendOp;



class ImageShard
{
    int srcX, srcY, srcWidth, srcHeight;
    int x, y;
    const PixelBuffer &fImage;

public:
    int fOriginX, fOriginY;
    
    ImageShard(const PixelBuffer &pb, int x, int y, int width, int height, int originX, int originY)
        : fImage(pb),
        srcX(x), srcY(y), srcWidth(width), srcHeight(height),
        fOriginX(originX), fOriginY(originY)
    {
        this->x = x;
        this->y = y;
    }

    int getWidth() const {return srcWidth;}
    int getHeight() const {return srcHeight;}

    int getOriginX() {return fOriginX;}
    int getOriginY() {return fOriginY;}

    void moveTo(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void draw()
    {
        //gAppSurface->blit(fImage, srcX, srcY, srcWidth, srcHeight, fOriginX, fOriginY, srcWidth, srcHeight);
        gAppSurface->blend(fImage, srcX, srcY, srcWidth, srcHeight, x, y, srcWidth, srcHeight, blendOp);
    }
};

class TimeAnimator
{
    StopWatch clock;
    double fDuration;

protected:
    TimeAnimator(double duration)
    :fDuration(duration)
    {
        clock.reset();
    }

public:
    virtual void reset()
    {
        clock.reset();
    }

    virtual void draw()
    {}
    
    virtual bool isDone()
    {
        return clock.seconds() >= fDuration;
    }

    virtual double portion()
    {
        return constrain(clock.seconds()/fDuration, 0, 1);
    }

    virtual void update()
    {}
};

class BarnDoorsOpen : public TimeAnimator
{
    PixelBuffer &fImage;
    ImageShard leftDoor;
    ImageShard rightDoor;
    ParamD fParamLeftX;
    ParamD fParamRightX;

public:
    BarnDoorsOpen(PixelBuffer &image, double duration)
        :TimeAnimator(duration),
        fImage(image),
        fParamLeftX(0,1, 0,-((int)image.getWidth())/2.0),
        fParamRightX(0,1, image.getWidth()/2, image.getWidth()),
        leftDoor(image, 0,0,image.getWidth()/2, image.getHeight(), 0,0),
        rightDoor(image, image.getWidth()/2,0,image.getWidth()/2, image.getHeight(), image.getWidth()/2,0)
    {
    }


    void draw()
    {
        update();
        leftDoor.draw();
        rightDoor.draw();

    }

    // Whenever we get a chance, update position of
    // the doors based on the time
    void update(){
        double p = portion();

        // update left door position
        int x = fParamLeftX(p);
        int y = leftDoor.fOriginY;
        leftDoor.moveTo(x, y);
        //printf("left door: %d %d\n", x, y);

        // update right door position
        x = fParamRightX(p);
        y = rightDoor.fOriginY;
        //printf("right door: %d %d\n", x, y);

        rightDoor.moveTo(x, y);
    }
};

BarnDoorsOpen *se1 = nullptr;


void draw()
{
    background(0xC0);


    if (se1 == nullptr) {
        return ;
    }

    se1->draw();
}

void setup()
{

    if (gargc < 1) {
        printf("You must specify a file to display.\n");
        halt();
    }

    printf("argv[1]: %s\n", gargv[1]);

    TargaMeta meta;
    apb = readFromFile(gargv[1], meta);

    if (nullptr == apb) {
        printf("image file not loaded.");
        halt();
    }


    createCanvas(apb->getWidth(), apb->getHeight());
    frameRate(30);

    se1 = new BarnDoorsOpen(*apb, 1);
    se1->reset();
}