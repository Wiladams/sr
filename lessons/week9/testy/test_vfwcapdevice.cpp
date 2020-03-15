
#include <stdio.h>

#include "VfwDriver.hpp"
#include "VfwCaptureDevice.hpp"

void main()
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