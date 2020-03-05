#pragma once

#include <stdio.h>

#include "List.hpp"
#include "iterator.hpp"

// Implementation of a Queue
// fist in first out
// push(val)
// val pop()
template <typename T>
class Queue 
{
public:
    bool enqueue(T value) {fList.pushRight(value); return true;}
    T dequeue() {return fList.popLeft();}
    T peek() {return fList.peekLeft();}
    int length() {return fList.length();}

    List<T> fList;
};

/*
    A queue iterator starts on the far left and 
    moves to the far right.
*/
template <typename T>
class QueueIterator : public IEnumerator<T>
{
    struct List<T> &fList;
    struct ListNode<T> *cursor;
    bool started = false;

public:
    QueueIterator(struct Queue<T> &aQueue)
        : fList(aQueue.fList)
    {
        cursor = fList.farLeft;
    }

    virtual bool moveNext()
    {
        //printf("moveNext: 1.0\n");
        // If we're totally empty, return immediately
        if (fList.farLeft == nullptr) {
            return false;
        }
        
        // Take care of case where it's the first element
        if (!started) {
            cursor = fList.farLeft;
            started = true;
            if (cursor != nullptr) {
                return true;
            } else {
                return false;
            }
        }

        //printf("moveNext: 3.0\n");
        // We're not on the first element, move to the next one
        cursor = cursor->right;
        if (cursor == nullptr) {
            return false;
        }

        //printf("moveNext: 5.0\n");
        // If we got to here, we have a valid next element
        return true;
    }

    virtual T getCurrent() const
    {
        // BUGBUG - if cursor is sitting on a null (invalid)
        // then throw exception
        return cursor->value;
    }

    virtual void reset()
    {
        cursor = nullptr;
    }
};
