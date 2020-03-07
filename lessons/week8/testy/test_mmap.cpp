#include <stdio.h>
#include "mmap.hpp"
#include "binstream.hpp"


void main()
{
    mmap m1 = mmap::create("testfile.txt");

    printf("isValid: %d size: %zd\n", m1.isValid(), m1.length());
    printf("=================================\n");
    printf("=================================\n");

    BinStream bs(m1.data, m1.size);
    while (!bs.isEOF()) {
        uint8_t c = bs.readOctet();
        printf("%c", c);
    }

    printf("\n=================================\n");
    printf("=================================\n");

}