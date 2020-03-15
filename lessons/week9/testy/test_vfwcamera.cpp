#include <cstdio>

#include "VfwCameraDevice.hpp"

/*
    void PrintCompressionType(uint compression)
    {
            Console.Write("Compression - ");
            switch (fBitmapInfo.bmiHeader.biCompression)
            {
                case GDI32.BI_BITFIELDS:
                    Console.WriteLine("Bitfields");
                    break;
                case GDI32.BI_JPEG:
                    Console.WriteLine("JPEG");
                    break;
                case GDI32.BI_PNG:
                    Console.WriteLine("PNG");
                    break;
                case GDI32.BI_RGB:
                    Console.WriteLine("RGB");
                    break;
                case GDI32.BI_RLE4:
                    Console.WriteLine("RLE4");
                    break;
                case GDI32.BI_RLE8:
                    Console.WriteLine("RLE8");
                    break;

                default:
                    Console.WriteLine("UNKNOWN");
                    break;
            }
    }
*/

void main()
{
    VfwCameraDevice cam(320, 240);

    if (!cam.isValid()) {
        printf("INVALID Camera.\n");
    }
    cam.selectVideoSource();
    
    cam.showPreviewWindow();
}