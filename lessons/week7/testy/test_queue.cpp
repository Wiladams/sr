#include <stdio.h>
#include "Queue.hpp"

// A simple functor to print an integer value
struct intPrint {void operator()(const int value) { printf("%d\n", value);}};


void test_iterator()
{
    printf("==== test_iterator ====\n");

    Queue<int> aq;

    // Fill the queue with some numbers
    for (int i=1; i<=100; i++) {
        aq.enqueue(i);
    }

    QueueIterator<int> qi(aq);

    printf("queue.length: %d\n", aq.length());


    intPrint ip;

    while (qi.moveNext()) {
        ip(qi.getCurrent());
    }

}

void test_simple()
{
        Queue<int> aList;

    aList.enqueue(1);
    aList.enqueue(2);
    aList.enqueue(3);

    // print out values
    while (aList.length() > 0) {
        printf("%d\n", aList.dequeue());
    }
}

void main()
{
    test_simple();
    test_iterator();

}