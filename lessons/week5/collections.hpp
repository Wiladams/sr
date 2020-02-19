typedef struct ListNode ListNode;

struct ListNode {
    void * value;
    ListNode * left;
    ListNode * right;

} ;

struct List {
    struct ListNode * farLeft;
    struct ListNode * farRight;

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


class ListIterator
{
protected:
    List & fList;



public:
    ListIterator(List &aList) : fList(aList) {}

    virtual void * next() = 0;
};

/*
    A queue iterator starts on the far left and 
    moves to the far right.
*/
class QueueIterator : public ListIterator
{
    struct ListNode *first;

public:
    QueueIterator(List &aList)
        : ListIterator(aList)
    {
        first = aList.farLeft;
    }

    void * next()
    {
        if (first == nullptr) {
            return nullptr;
        }

        void * nodeValue = first->value;
        first = first->right;

        return nodeValue; 
    }
};

class Queue
{
private:
    List fList;

public:
    bool enqueue(void *value) {fList.pushRight(value); return true;}
    void * dequeue() {return fList.popLeft();}
    int length() {return fList.length();}
    
    QueueIterator values() 
    {
        return QueueIterator(fList);
    }
};