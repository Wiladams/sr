#include "collections.hpp"
#include <stdio.h>

void testStack()
{
    printf("==== testStack ====\n");

    List ls;
    
    ls.pushRight("this");
    ls.pushRight("is");
    ls.pushRight("how");
    ls.pushRight("we");
    ls.pushRight("do");
    ls.pushRight("it");

    while (ls.length() > 0 ) {
        printf("%s\n", (char*)ls.popRight());
    }
}

void testQueue()
{
    printf("==== testQueue ====\n");

    List ls;
    
    ls.pushRight("this");
    ls.pushRight("is");
    ls.pushRight("how");
    ls.pushRight("we");
    ls.pushRight("do");
    ls.pushRight("it");

    //printf("%d %s\n", ls.length(), (char*)ls.popLeft());
    //printf("%d %s\n", ls.length(), (char*)ls.popLeft());
    //printf("%d %s\n", ls.length(), (char*)ls.popLeft());

    while (ls.length() > 0 ) {
        int l = ls.length();
        printf("%d %s\n", l, (char*)ls.popLeft());
    }
}


void main()
{
    testQueue();
    //testStack();
}