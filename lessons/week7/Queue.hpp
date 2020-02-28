#pragma once

#include "List.hpp"

// Implementation of a Queue
// fist in first out
// push(val)
// val pop()
template <typename T>
class Queue
{
public:
    List<T> fList;

    bool enqueue(T value) {fList.pushRight(value); return true;}
    T dequeue() {return fList.popLeft();}
    T peek() {return fList.peekLeft();}
    int length() {return fList.length();}

    
};
