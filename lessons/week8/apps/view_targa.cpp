#include "p5.hpp"
#include "codec_targa.hpp"
#include "mmap.hpp"

#include <stdio.h>


PixelBuffer *pb = nullptr;

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

void draw()
{
    if (pb!= nullptr) {
        image(*pb, 10,10);
    }
}

void setup()
{
    createCanvas(400,400);

    pb = readFromFile("data\\FLAG_B32.tga");

    if (pb != nullptr) {
        printf("Size: %d X %d\n", pb->getWidth(), pb->getHeight());
    }
}