
#pragma once

#include "grtypes.hpp"
#include "PixelBuffer.hpp"

#include <stdio.h>
#include <stdlib.h>


/*
int write_PPM_ascii(const char *filename, PixelBuffer *fb)
{
	FILE * fp = fopen(filename, "wb");

	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P3\n%d %d\n255\n", int(fb->frame.width), int(fb->frame.height));

	// write the individual pixel values in binary form
	uint32_t * pixelPtr = (uint32_t *)fb->data;
	uint8_t rgb[3];

	for (int row = 0; row < fb->frame.height; row++) {
		for (int col = 0; col < fb->frame.width; col++){
			rgb[0] = GET_R(*pixelPtr);
			rgb[1] = GET_G(*pixelPtr);
			rgb[2] = GET_B(*pixelPtr);
			fprintf(fp, "%3d %3d %3d  ", rgb[0], rgb[1], rgb[2]);
		}
		pixelPtr += fb->pixelpitch;
		fprintf(fp,"\n");
	}


	fclose(fp);

	return 0;
}
*/



class PBM
{
public:
	static bool writePPMBinary(const char *filename, const PixelBuffer &pb)
	{
		FILE * fp = fopen(filename, "wb");
	
		if (!fp)
			return false;

		// write out the image header
		fprintf(fp, "P6\n%d %d\n255\n", pb.getWidth(), pb.getHeight());
	
		// write the individual pixel values in binary form
		for (size_t row = 0; row < pb.getHeight(); row++) 
		{
			for (size_t col = 0; col < pb.getWidth(); col++)
			{
				fwrite(pb.getPixel(col, row).data, 3, 1, fp);
			}
		}

		fclose(fp);
	
		return true;
	}
};
