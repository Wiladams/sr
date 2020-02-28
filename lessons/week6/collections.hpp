#pragma once

//#include <stdio.h>

//typedef struct ListNode ListNode;

struct ListNode {
    void * value;
    ListNode * left;
    ListNode * right;

} ;

struct List {
    ListNode * farLeft;
    ListNode * farRight;

    List() : farLeft(nullptr), farRight(nullptr){}

    int length() {
        if (farLeft == nullptr) {
            return 0;
        }

        int counter = 1;
        struct ListNode *cursor = farLeft;
        while (cursor->right != nullptr) {
            counter = counter + 1;
            cursor = cursor->right;
        }

        return counter;
    }

    void pushRight(void *value) {
        struct ListNode * newNode = new struct ListNode;

        // setup the new node
        newNode->value = value;
        newNode->left = farRight;
        newNode->right = nullptr;

        if (farRight != nullptr) {
            farRight->right = newNode;
        }

        farRight = newNode;

        if (farLeft == nullptr) {
            farLeft = newNode;
        }

    }

    void * popRight()
    {
        struct ListNode * valueNode = farRight;
        if (valueNode == nullptr) {
            return nullptr;
        }

        void * retValue = valueNode->value;
        if (valueNode->left != nullptr) {
            farRight = valueNode->left;
            farRight->right = nullptr;
        } else {
            farRight = nullptr;
        }


        if (farLeft == valueNode) {
            farLeft = nullptr;
        }
    
        return retValue;
    }

    void * popLeft()
    {
        struct ListNode * valueNode = farLeft;
        if (valueNode == nullptr) {
            return nullptr;
        }

        void * retValue = valueNode->value;
        if (valueNode->right != nullptr) {
            farLeft = valueNode->right;
            farLeft->left = nullptr;
        } else {
            farLeft = nullptr;
        }

        if (farRight == valueNode) {
            farRight = nullptr;
        }

        return retValue;
    }
};


// Implementation of a Queue
// fist in first out
// push(val)
// val pop()
class Queue
{
public:
    List fList;

    bool enqueue(void *value) {fList.pushRight(value); return true;}
    void * dequeue() {return fList.popLeft();}
    int length() {return fList.length();}
};


// Implementation of a Stack
// first in last out
// enqueue(val)
// val dequeue()
class Stack
{
public:
    List fList;

    bool push(void *value) {fList.pushRight(value); return true;}
    void * pop() {return fList.popRight();}
    int length() {return fList.length();}
};

/*
    Iterators
*/
class ListIterator
{
protected:
    struct ListNode *first;

public:
    ListIterator(struct ListNode *aNode) : first(aNode) {}

    virtual bool next(void **value) = 0; //{return false;};
};

/*
    A queue iterator starts on the far left and 
    moves to the far right.
*/
class QueueIterator
{
    struct ListNode *first;

public:
    QueueIterator(struct Queue &aQueue)
        : first(aQueue.fList.farLeft)
    {}

    virtual bool next(void **value)
    {
        if (first == nullptr) {
            return false;
        }

        *value = first->value;
        first = first->right;
        return true;
    }
};


class StackIterator : public ListIterator
{
public:
    StackIterator(struct Stack &aStack)
        : ListIterator(aStack.fList.farRight)
    {}

    bool next(void **value)
    {
        if (first == nullptr) {
            return false;
        }

        *value = first->value;
        first = first->left;

        return true;
    }
};

