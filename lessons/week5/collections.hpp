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
        ListNode * valueNode = farRight;
        if (farRight == nullptr) {
            return nullptr;
        }

        void * retValue = valueNode->value;
        if (valueNode->left != nullptr) {
            farRight = valueNode->left;
            farRight->right = nullptr;
        }


        if (farLeft == valueNode) {
            farLeft = nullptr;
        }
    
        return retValue;
    }
};