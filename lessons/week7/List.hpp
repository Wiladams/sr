#pragma once

template <typename T>
struct ListNode {
    T value;
    ListNode<T> *left;
    ListNode<T> *right;

    ListNode(T value, ListNode<T> *left, ListNode<T> *right)
        : left(left), right(right), value(value)
    {
    }
} ;

template <typename T>
struct List {
    ListNode<T> * farLeft;
    ListNode<T> * farRight;

    List() : farLeft(nullptr), farRight(nullptr){}

    int length() {
        if (farLeft == nullptr) {
            return 0;
        }

        int counter = 1;
        struct ListNode<T> *cursor = farLeft;
        while (cursor->right != nullptr) {
            counter = counter + 1;
            cursor = cursor->right;
        }

        return counter;
    }

    void pushRight(T value) {
        ListNode<T> * newNode = new ListNode<T>(value, farRight, nullptr);

        if (farRight != nullptr) {
            farRight->right = newNode;
        }

        farRight = newNode;

        if (farLeft == nullptr) {
            farLeft = newNode;
        }

    }

    T popRight()
    {
        ListNode<T> * valueNode = farRight;
        if (valueNode == nullptr) {
            return T();
        }

        T retValue = valueNode->value;
        if (valueNode->left != nullptr) {
            farRight = valueNode->left;
            farRight->right = nullptr;
        } else {
            farRight = nullptr;
        }


        if (farLeft == valueNode) {
            farLeft = nullptr;
        }
        
        // Make sure we don't have a memory leak
        delete valueNode;

        return retValue;
    }

    T peekRight() {
        if (farRight == nullptr) {
            return T();
        }

        return farRight.value;
    }

    T popLeft()
    {
        ListNode<T> * valueNode = farLeft;
        if (valueNode == nullptr) {
            return T();
        }

        T retValue = valueNode->value;
        if (valueNode->right != nullptr) {
            farLeft = valueNode->right;
            farLeft->left = nullptr;
        } else {
            farLeft = nullptr;
        }

        if (farRight == valueNode) {
            farRight = nullptr;
        }
        
        // Make sure we don't have a memory leak
        delete valueNode;

        return retValue;
    }

    T peekLeft()
    {
        if (farLeft == nullptr) {
            return T();
        }

        return farLeft.value;
    }
};
