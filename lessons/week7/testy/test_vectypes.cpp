#include <stdio.h>
#include "vec_types.hpp"


void main()
{
    vec3 v1 = {10,20,30};
    vec3 v2 = {20,30,40};
    vec3 v3 = v1 + v2;

    printf("add: %3.2f %3.2f %3.2f\n", v3.x, v3.y, v3.z);
}