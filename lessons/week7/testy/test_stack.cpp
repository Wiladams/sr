#include <stdio.h>
#include "Stack.hpp"

void main()
{
    Stack<int> aList;

    aList.push(1);
    aList.push(2);
    aList.push(3);

    while (aList.length() > 0) {
        printf("%d\n", aList.pop());
    }

}
