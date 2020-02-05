#include "FrameBuffer.hpp"
#include "DrawingContext.hpp"
#include "colors.hpp"
#include "pbm.hpp"

void printPoint(const char *name, const Point2D &p)
{
    printf("%s ==> x: %d  y: %d\n", name, p.x, p.y);
}

void main()
{
    Point2D a(10,20);
    Point2D b(23,33);

    Point2D c(a+b);
    printPoint("c", c);
}