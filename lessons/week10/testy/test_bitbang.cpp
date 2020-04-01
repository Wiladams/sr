#include <cstdio>
#include "bitbang.hpp"

void test_swap32()
{

}

void main()
{
    uint32_t innum = 0xfeedface;
    uint32_t outnum = swapUInt32(innum);

    printf("IN (0xfeedface): 0x%8x OUT (0xcefaedfe): 0x%8x \n", innum, outnum);
}