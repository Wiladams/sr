#include "PixelBuffer.hpp"



void main()
{
    // Should not be able to create an instance 
    // of one of these because it has pure virtual methods
    PixelBuffer pb(640, 480);

}