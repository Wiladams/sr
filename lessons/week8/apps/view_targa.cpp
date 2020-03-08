#include "p5.hpp"
#include "codec_targa.hpp"
#include "mmap.hpp"

#include <stdio.h>


PixelBuffer *apb = nullptr;

void printMeta(TargaMeta &meta)
{
    printf("Size: %d X %d\n", meta.header.Width, meta.header.Height);
    printf("Pixel Depth: %d\n", meta.header.PixelDepth);
    printf("ImageType: %d\n", meta.header.ImageType);
    printf("Orientation: H: %d V: %d\n", meta.header.HorizontalOrientation, meta.header.VerticalOrientation);

}

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

    PixelBuffer * abuff = readFromStream(bs, meta);
    printMeta(meta);

    return abuff;
}

void draw()
{
    if (apb != nullptr) {
        image(*apb, 0,0);
    }
    noLoop();
}

void setup()
{
    createCanvas(640,480);

    //apb = readFromFile("data\\FLAG_B16.tga");
    //apb = readFromFile("data\\FLAG_B24.tga");
    //apb = readFromFile("data\\FLAG_B32.tga");
    //apb = readFromFile("data\\MARBLES.tga");
    //apb = readFromFile("data\\XING_T32.tga");
    apb = readFromFile("data\\indexed_UL.tga");

    if (apb != nullptr) {
        printf("Size: %d X %d\n", apb->getWidth(), apb->getHeight());

    }
}