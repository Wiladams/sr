This directory contains some simple app cases based on using 
the p5 model of programming.

You can compile them using the following simple command line:

cl /I.. app_xxx.cpp

where the 'xxx' is replaced with proper name of file.

To use the P5 programming model, you must adhere to a few rules.

1) Your top level program must be named with a '.cpp' file extension

Ex: app_plotbezier.cpp

2) Your program MUST contain one, and only one createCanvas() function calls.
This function establishes the size of the window you will display on the screen, so it must be called, typicaly in the setup() function.

3) You MUST create a setup() function, and create a canvas in there.

```C
// Bare minimum P5 program
#include "p5.hpp"

void setup()
{
    createCanvas(640, 480);
}
```