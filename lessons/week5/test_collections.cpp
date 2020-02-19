#include "collections.hpp"
#include <stdio.h>


void main()
{
    List ls;

    // pushRight, popRight implements a stack
    ls.pushRight("this");
    ls.pushRight("is");
    ls.pushRight("how");
    ls.pushRight("we");
    ls.pushRight("do");
    ls.pushRight("it");

    printf("length: %d (6)\n", ls.length());

    while (ls.length() > 0 ) {
        printf("%s\n", (char*)ls.popRight());
    }
}