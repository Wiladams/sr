#pragma once

#include <stdint.h>

#if BUILD_AS_DLL
  #define GPROC_API		__declspec(dllexport)
#else
//  #define GPROC_API		__declspec(dllimport)
  #define GPROC_API
#endif

//#define GPROC_EXPORT		__declspec(dllexport)
#define GPROC_EXPORT



typedef struct RGBA_t {
    union {
        uint32_t intValue;
        uint8_t data[4];
        struct {uint8_t r, g, b, a;};
    };
} PixRGBA;


typedef struct PixelBuffer {
    size_t height;          // How many rows
    size_t width;           // How many columns
    size_t pixelpitch;      // how many pixels between rows

    PixRGBA * data;     // The actual data
} PixelBuffer;

