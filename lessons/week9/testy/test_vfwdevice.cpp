#include "VfwDeviceManager.hpp"
#include <stdio.h>

void main()
{
    VfwDeviceManager mgr;

    for (int i=0;i<10;i++)
    {
        if (mgr[i].isValid()) {
            printf("mgr[%d] Name ==> %s Version ==> %s\n", i, mgr[i].fName, mgr[i].fVersion);
        }
    }
}