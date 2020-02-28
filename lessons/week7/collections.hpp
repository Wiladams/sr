#include <stdio.h>






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

