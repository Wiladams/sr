#include "grtypes.h"

typedef struct PixelBuffer {
    size_t height;          // How many rows
    size_t width;           // How many columns
    size_t pixelpitch;      // how many pixels between rows

    PixRGBA * data;     // The actual data
} PixelBuffer;