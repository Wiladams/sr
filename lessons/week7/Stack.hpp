#pragma once

#include "List.hpp"

// Implementation of a Stack
// first in last out
// enqueue(val)
// val dequeue()
template<typename T>
class Stack
{
public:
    List<T> fList;

    bool push(T value) {fList.pushRight(value); return true;}
    T pop() {return fList.popRight();}
    int length() {return fList.length();}
};
