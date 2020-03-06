#include <stdio.h>
#include "binstream.hpp"

void testInt()
{
    size_t size = 256;
    uint8_t data[256];

    BinStream s(data, size);

    s.writeInt8(-1);
    s.writeUInt8(-1);

    s.seek(0);

    printf("int8: %d \n", s.readInt8());
    printf("uint8: %d \n", s.readUInt8());

}

void testStringZ()
{
    printf("==== testStringZ ====\n");
    const size_t size = 256;
    uint8_t data[size];

    BinStream s(data, size);

    s.writeStringZ("Hello, World!");

    printf("Length: %I64d\n", s.tell());
    printf("Remain: %I64d\n", s.remaining());

    char str[size];
    s.seek(0);
    int len = s.readStringZ(size, str);
    printf("String: %d %s\n",len, str);
}

void testReadline()
{
    printf("==== testReadLine ====\n");
    char * str = "the\r\nquick brown\r\nfox jumped\nover the\nlazy\ndog";
    int size = strlen(str);

    BinStream s((uint8_t *)str, size);

    char buff[256];
    while (!s.isEOF()) {
        size_t len = s.readLine(256, buff);
        printf("%s\n", buff);
    }
}

void main()
{
    //testStringZ();
    //testInt();
    testReadline();
}