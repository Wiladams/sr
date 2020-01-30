#include "grtypes.h"


#ifdef __cplusplus
extern "C" {
#endif

// PixelBuffer Handling
GPROC_API bool PB_init(PixelBuffer &pb, size_t width, size_t height);
GPROC_API bool PB_clear(PixelBuffer &pb);
GPROC_API bool PB_setPixel(PixelBuffer &pb, size_t x, size_t y, PixRGBA c);
PixRGBA PB_getPixel(PixelBuffer &pb, size_t x, size_t y);

GPROC_API bool PB_HorizontalLine(PixelBuffer &pb, const size_t row, const size_t col, 
    const size_t numberOfPixels, const PixRGBA c);

GPROC_API bool PB_Rectangle(PixelBuffer &pb, size_t x, size_t y, 
    size_t width, size_t height, const PixRGBA c);

// PBM Handling
//GPROC_API bool PPM_read_binary(const char *filename, PixelBuffer &fb);			// will read P6
GPROC_API bool write_PPM_binary(const char *filename, const PixelBuffer &fb);	// write a P6 file
//GPROC_API bool write_PPM_ascii(const char *filename, PixelBuffer &fb);		// write a P3 file

#ifdef __cplusplus
}
#endif
