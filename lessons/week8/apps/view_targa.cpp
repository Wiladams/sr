#include "p5.hpp"
#include "codec_targa.hpp"
#include "mmap.hpp"

#include <stdio.h>


PixelBuffer *apb = nullptr;
PixelSRCOVER blendOp;

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
    background(0);
    
    if (apb != nullptr) {
        //image(*apb, 0,0);

        gAppSurface->blend(*apb, 0, 0, apb->getWidth(), apb->getHeight(), 0, 0, width, height, blendOp);
    }
    noLoop();
}

/*
    Some files to try:

    FLAG_B16.tga
    FLAG_B24.tga
    FLAG_B32.tga
    
    MARBLES.tga
    
    XING_T32.tga

    indexed_UL.tga
    indexed_UR.tga
    indexed_LL.tga
    indexed_LR.tga

More from here:
http://people.math.sc.edu/Burkardt/data/tga/tga.html
https://github.com/tmyroadctfig/com.realityinteractive.imageio.tga
https://github.com/DEF7/TGA2STL
https://github.com/brandonprry/tga-fuzz

*/

void setup()
{
    printf("argc: %d\n", gargc);
    if (gargc < 1) {
        printf("You must specify a file to display.\n");
        halt();
    }

    printf("argv[1]: %s\n", gargv[1]);

    apb = readFromFile(gargv[1]);


    if (apb != nullptr) {
        //printf("Size: %d X %d\n", apb->getWidth(), apb->getHeight());
        createCanvas(apb->getWidth(),apb->getHeight());
        //createCanvas(320,240);
    } else {
        halt();
    }
}