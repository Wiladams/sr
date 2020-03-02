#include <stdio.h>
#include "Stack.hpp"

void printStack(const Stack<int> &s)
{
    for (int i=0; i<s.length(); i++)
    {
        printf("%d ==> %d\n", i, s[i]);
    }
}

void test_basic()
{
    printf("==== test_basic ====\n");

    Stack<int> aList;

    aList.push(1);
    aList.push(2);
    aList.push(3);

    while (aList.length() > 0) {
        printf("%d\n", aList.pop());
    }
}

void test_nth()
{
    printf("==== test_nth ====\n");
    Stack<int> s;
    //  load up the stack with some values
    for (int i =99; i>=0 ;i--) {
        s.push(i);
    }

    printf("top: %d\n", s.top());
    printf("0: %d\n", s.nth(0));
    printf("10: %d\n", s.nth(10));
    printf("99: %d\n", s.nth(99));
    printf("100: %d\n", s.nth(100));

}

void test_copy()
{
    printf("==== test_copy ====\n");
    Stack<int> s;
    s.push(3);
    s.push(2);
    s.push(1);
    s.push(0);

    s.copy(3);

    printStack(s);
}

void main()
{
    test_basic();
    test_nth();
    test_copy();
}
