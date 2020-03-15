#include "p5.hpp"

#include <stdio.h>

#include "VfwDriver.hpp"
#include "VfwCaptureDevice.hpp"

void setupCamera()
{
    VfwCaptureDevice device;
    VfwDeviceDriver driver(0);

    printf("Driver: [%d - %d] %s\n", driver.getIndex(), driver.isValid(), driver.getName());

    if (!device.connectToDriver(driver)) {
        printf("failed to connect to driver\n");
        return ;
    }

    device.selectVideoSource();
    device.selectVideoFormat();
}

void setup()
{
    createCanvas(800, 600);
    setupCamera();

    noLoop();
}