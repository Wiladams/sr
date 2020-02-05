#include "PixelBufferRGBA32.hpp"


void main()
{
    // Should not be able to create an instance 
    // of one of these because it has pure virtual methods
    PixelBufferRGBA32 pb(640, 480);

}