#include "collections.hpp"
#include <stdio.h>

void testQueue()
{
    printf("==== testQueue ====\n");

    Queue q;
    
    q.enqueue("this");
    q.enqueue("is");
    q.enqueue("how");
    q.enqueue("we");
    q.enqueue("do");
    q.enqueue("it");

    while (q.length() > 0 ) {
        int l = q.length();
        printf("%d %s\n", l, (char*)q.dequeue());
    }

}

void testIterator()
{
    printf("==== testIterator ====\n");

    Queue q;
    
    q.enqueue("this");
    q.enqueue("is");
    q.enqueue("how");
    q.enqueue("we");
    q.enqueue("do");
    q.enqueue("it");

    
    QueueIterator qi(q);
    void * val = nullptr;

    while (qi.next(&val)) {
        printf("val: %s\n", (char *)val);
    }
}

void main()
{
    testQueue();
    testIterator();
}