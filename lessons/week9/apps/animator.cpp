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
    double fRemaining;


protected:
    bool fIsRunning = false;

    TimeAnimator(double duration)
        :fDuration(duration),
        fIsRunning(false)
    {
        stop();
    }

public:
    bool isRunning() {
        return fIsRunning;
    }

    virtual void pause()
    {
        fRemaining = fDuration - clock.seconds();
    }

    virtual void play()
    {
        fIsRunning = true;
        // set the clock to the amount remaining
        // and continue
    }

    virtual void reset(){clock.reset();}
    virtual void stop(){fIsRunning = false; reset();}
    virtual bool isDone(){return clock.seconds() >= fDuration;}
    virtual double portion() {return constrain(clock.seconds()/fDuration, 0, 1);}
    virtual void update(){}
    virtual void draw(){}
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
        leftDoor(image, 0,0,(image.getWidth()/2)+1, image.getHeight(), 0,0),
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
        // If th animation is not running, then
        // simply return instead of updating
        if (!isRunning()) {
            return ;
        }

        double p = portion();

        // update left door position
        int x = fParamLeftX(p);
        int y = leftDoor.fOriginY;
        leftDoor.moveTo(x, y);
        //printf("left door: %d %d\n", x, y);

        // update right door position
        x = fParamRightX(p);
        y = rightDoor.fOriginY;
        rightDoor.moveTo(x, y);

        if (p >= 1.0) {
            fIsRunning = false;
        }
    }

    void play()
    {
        reset();
        fIsRunning = true;
    }
};

BarnDoorsOpen *se1 = nullptr;


void mouseReleased(const MouseEvent &e)
{
    if (se1->isRunning()) {
        se1->stop();
    } else {
        se1->play();
    }

}

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