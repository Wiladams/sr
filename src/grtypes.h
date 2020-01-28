#include <stdint.h>

/* create DPCOLORVAL (0xAABBGGRR format)*/
#define DPARGB(a,r,g,b)	((DPCOLORVAL)(((unsigned char)(r)|\
				(((uint32_t)(unsigned char)(g))<<8))|\
				(((uint32_t)(unsigned char)(b))<<16)|\
				(((uint32_t)(unsigned char)(a))<<24)))
#define DPRGB(r,g,b)	DPARGB(255,(r),(g),(b))		/* argb 255 alpha*/
#define DP0RGB(r,g,b)	DPARGB(0,(r),(g),(b))		/* rgb 0 alpha*/

#define REDVALUE(rgb)	((rgb) & 0xff)
#define GREENVALUE(rgb) (((rgb) >> 8) & 0xff)
#define BLUEVALUE(rgb)	(((rgb) >> 16) & 0xff)
#define ALPHAVALUE(rgb)	(((rgb) >> 24) & 0xff)


typedef struct PixelBuffer {
    size_t rows;           // How many rows
    size_t columns;        // How many columns
    
    uint32_t * data;     // The actual data
} PixelBuffer;

