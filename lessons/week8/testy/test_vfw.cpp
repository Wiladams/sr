#include "w32.hpp"
#include <stdio.h>

void main()
{
    uint32_t ver = VideoForWindowsVersion();

    printf("ver: 0x%x\n", ver);

}