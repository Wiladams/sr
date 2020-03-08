#include "p5.hpp"
#include "codec_targa.hpp"
#include "mmap.hpp"

#include <stdio.h>


PixelBuffer *apb = nullptr;

void printMeta(TargaMeta &meta)
{
    printf("ID Length: %d\n", meta.header.IDLength);
    printf("Size: %d X %d\n", meta.header.Width, meta.header.Height);
    printf("Pixel Depth: %d\n", meta.header.PixelDepth);
    printf("ImageType: %d\n", meta.header.ImageType);
    printf("Color Map Type: %d\n", meta.header.ColorMapType);
    printf("Color Map Depth: %d\n", meta.header.CMapDepth);
    printf("Color Map Start: %d\n", meta.header.CMapStart);
    printf("Color Map Length: %d\n", meta.header.CMapLength);
    printf("Orientation: H: %d V: %d\n", meta.header.HorizontalOrientation, meta.header.VerticalOrientation);

    printf("Compressed: %d\n", meta.header.Compressed);

    printf("Extension: %d\n", meta.footer.isExtended);
    if (meta.footer.isExtended) {
        printf("  Developer Directory Offset: 0x%x\n", meta.footer.DeveloperDirectoryOffset);
        printf("  Extension Offset: 0x%x\n", meta.footer.ExtensionAreaOffset);
        printf("  Signature: %s\n", meta.footer.Signature);

    }
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
    //apb = readFromFile("data\\FLAG_B16.tga");
    //apb = readFromFile("data\\FLAG_B24.tga");
    //apb = readFromFile("data\\FLAG_B32.tga");
    //apb = readFromFile("data\\MARBLES.tga");
    apb = readFromFile("data\\XING_T32.tga");
    //apb = readFromFile("data\\indexed_UL.tga");

    if (apb != nullptr) {
        printf("Size: %d X %d\n", apb->getWidth(), apb->getHeight());
        createCanvas(apb->getWidth(),apb->getHeight());
    } else {
        halt();
    }
}