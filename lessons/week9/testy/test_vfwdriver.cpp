#include "VfwDriver.hpp"
#include <stdio.h>

void main()
{

    for (int i=0;i<10;i++)
    {
        VfwDeviceDriver driver(i);
        if (driver.isValid()) {
            printf("Driver[%d] NAME ==> %s VERSION ==> %s\n", 
                driver.getIndex(), driver.getName(), driver.getVersion());
        }
    }
}