#include "bitbang.hpp"
#include "grtypes.hpp"
#include <stdio.h>



void testBITMASK()
{
    printf("==== testBITMASK ====\n");
    uint16_t src16 = 0b0000000000011111;


    uint32_t mask = BITMASK(0,4);

    printf("MASK: (0,4) %d\n", mask);
}

void testBITSVALUE()
{
    printf("==== testBITSVALUE ====\n");

    uint16_t src16 = 0b0000000000011111;

    uint8_t red = BITSVALUE(src16, 0, 4);

    printf("RED: %d\n", red);

}

void testDecode()
{
    printf("==== testDecode ====\n");

    uint16_t src16 = 0b0000000000011111;
    
    PixRGBA pix;


    pix.red = (BITSVALUE(src16,0,4) << 3);
    pix.green = (BITSVALUE(src16,5,9) << 3);
    pix.blue = (BITSVALUE(src16,10,14) << 3);
    pix.alpha = 255;

    printf("pix: (%d, %d, %d, %d)\n", pix.red, pix.green, pix.blue, pix.alpha);
}

void main()
{
    testBITMASK();
    testBITSVALUE();
    testDecode();

}