#include <stdio.h>
#include "Queue.hpp"

void main()
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