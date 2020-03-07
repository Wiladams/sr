
#include <stdio.h>
#include "codec_targa.hpp"
#include "mmap.hpp"

PixelBuffer * readFromFile(const char *filename)
{
    mmap fmap = mmap(filename);
    if (!fmap.isValid()) {
        printf("Could not map file: %s\n", filename);
        return nullptr;
    }

    BinStream bs(fmap.getPointer(), fmap.length(), 0, true );

    if (!bs.isValid()) {
        printf("BinaryStream not valid.\n");
        return nullptr;
    }

    TargaMeta meta;
    PixelBuffer *pb = readFromStream(bs, meta);

    return pb;
}

void main()
{
    PixelBuffer * pb = readFromFile("data\\FLAG_B32.tga");

    if (pb != nullptr) {
        printf("Size: %d X %d\n", pb->getWidth(), pb->getHeight());
    }
}