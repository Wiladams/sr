#include <stdio.h>
#include "maths.hpp"

void printVec2(const vec2 &v)
{
    printf("vec2(%3.2f, %3.2f);\n", v.x, v.y);
}

void testadd()
{
    printf("==== testadd ====\n");

    vec2 v1 = {20,30};
    vec2 v2 = {13,25};

    vec2 c = v1+v2;

    printVec2(c);
}

void testmul()
{
    printf("==== testmul ====\n");
    
    vec2 v1 = {20,30};
    v1 *= 3;

    printVec2(v1);
}

void main()
{
    testadd();
    testmul();
}