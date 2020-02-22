#include "p5.hpp"
#include <stdio.h>

void setup()
{
    for (int i=1; i<=100;i++) {
        printf("%d == %1.3f\n", i, random());
    }

    halt();
}