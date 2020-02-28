#include <stdio.h>
#include "List.hpp"

void main()
{
    List<int> aList;

    aList.pushRight(1);
    aList.pushRight(2);
    aList.pushRight(3);

    // Test it like a stack
    while (aList.length() > 0) {
        printf("%d\n", aList.popRight());
    }

}