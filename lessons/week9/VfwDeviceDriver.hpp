#pragma once

#include <string.h>
#include <cstdio>

#include "w32.hpp"
#include "w32_video.hpp"

class VfwDeviceDriver
{
public:
    unsigned int fDriverIndex;
    char fName[100];
    char fVersion[100];
    bool fIsValid;

    VfwDeviceDriver()
        : fIsValid(false),
        fDriverIndex(0)
    {
        memset(fName, 0, sizeof(fName));
        memset(fVersion, 0, sizeof(fVersion));
    }

    VfwDeviceDriver(const VfwDeviceDriver &rhs)
        :fDriverIndex(rhs.fDriverIndex),
        fIsValid(rhs.fIsValid)
    {
        strncpy_s(fName, sizeof(fName),rhs.fName, sizeof(rhs.fName) );
        strncpy_s(fVersion, sizeof(fVersion),rhs.fVersion, sizeof(rhs.fVersion) );
    }

    VfwDeviceDriver(const unsigned int driverIndex)
        : fIsValid(false)
    {
        fDriverIndex = 0;
        memset(fName, 0, sizeof(fName));
        memset(fVersion, 0, sizeof(fVersion));

        fIsValid = capGetDriverDescriptionA (driverIndex, fName, sizeof(fName), fVersion, sizeof(fVersion)) == 1;
        fDriverIndex = driverIndex;
    }

    bool isValid() const {return fIsValid;}
    unsigned int getIndex() const {return fDriverIndex;}
    const char * getName() const {return fName;}
    const char * getVersion() const {return fVersion;}
    
    char * toString(int n, char *buff)
    {
        int nWritten = snprintf (buff, n, "%s %s", fName, fVersion);
        return buff;
    }
};






