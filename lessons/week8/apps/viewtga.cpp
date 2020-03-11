/*
    A simple program to view Truevision Targa (.tga)
    Some files to try:

    More from here:
    http://people.math.sc.edu/Burkardt/data/tga/tga.html
    https://github.com/tmyroadctfig/com.realityinteractive.imageio.tga
    https://github.com/DEF7/TGA2STL
    https://github.com/brandonprry/tga-fuzz
*/
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
        //printf("  Signature: %s\n", meta.footer.Signature);

    }
}



void draw()
{
    background(0);


    if (apb != nullptr) {
        image(*apb, 0,0);
        //gAppSurface->blend(*apb, 0, 0, apb->getWidth(), apb->getHeight(), 0, 0, width, height, blendOp);

        // offset clipped blit
        //gAppSurface->blit(*apb, 0, 0, apb->getWidth(), apb->getHeight(), 100, 100, width, height);
    }
    noLoop();

}



void setup()
{
    printf("argc: %d\n", gargc);
    if (gargc < 1) {
        printf("You must specify a file to display.\n");
        halt();
    }

    printf("argv[1]: %s\n", gargv[1]);

    TargaMeta meta;
    apb = readFromFile(gargv[1], meta);


    if (apb != nullptr) {
        printMeta(meta);
        int dx = MAX(meta.header.Width, 320);
        int dy = MAX(meta.header.Height, 240);

        createCanvas(dx,dy);
    } else {
        halt();
    }
}