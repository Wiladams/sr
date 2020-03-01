#include <stdio.h>

template <typename T>
struct IEnumerator {
    virtual bool moveNext() = 0;
    virtual void reset() = 0;
    T getCurrent()
    {
        return T();
    }
};

template <typename T>
struct IIterable {
    virtual bool isEmpty() const = 0;
    virtual T& next() = 0;
    virtual IEnumerator<T> GetEnumerator() = 0;

};

struct AFunction
{
    void operator()(void)
    {
        printf("FUNCTOR\n");
    }
};

void each(AFunction aFunc, IIterable &aIterator)
{
    while (!aIterator.empty())
        {
            // perform aFunc on each item from iterator
        }
}

void main()
{
    AFunction func;

    func();
}