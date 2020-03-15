/*
    This app uses the VfwCameraDevice to display 
    the camera preview on the screen in its
    own window.
*/

#include "p5.hpp"

#include <stdio.h>

#include "VfwCameraDevice.hpp"

void setupCamera()
{
    VfwCameraDevice camera(640, 480);
    VfwDeviceDriver driver(0);

    printf("Driver: [%d - %d] %s\n", driver.getIndex(), driver.isValid(), driver.getName());

    if (!camera.connectToDriver(driver)) {
        printf("failed to connect to driver\n");
        return ;
    }

    camera.selectVideoSource();
    camera.selectVideoFormat();

    
    camera.setPreviewRate(33);
    camera.setPreviewScale(1);

    camera.startPreview();
}

void setup()
{
    createCanvas(800, 600);
    setupCamera();

    noLoop();
}