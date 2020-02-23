#include "collections.hpp"
#include <stdio.h>

void testStack()
{
    printf("==== test Stack ====\n");

    Stack q;
    
    q.push("this");
    q.push("is");
    q.push("how");
    q.push("we");
    q.push("do");
    q.push("it");

    void * val = q.pop();
    while (val != nullptr) {
        printf("stack pop: %s\n", (char *)val);
        val = q.pop();
    }
}


void testIterator()
{
    printf("==== test Stack Iterator ====\n");

    Stack s;
    
    s.push("this");
    s.push("is");
    s.push("how");
    s.push("we");
    s.push("do");
    s.push("it");

    
    StackIterator si(s);


    void * val = nullptr;
    while (si.next(&val)) {
        printf("stack val: %s\n", (char *)val);
    }
}

void main()
{
    testStack();
    testIterator();
}
