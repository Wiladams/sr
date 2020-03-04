Concepts
========

functor - An object that looks and behaves like a function.  This 
can be achieved in C++ by implementing operator().

The simplest implementation of a functor looks like this

```c++
struct Functor
{
    void operator()(void)
    {
        // do something interesting here
        printf("FUNCTOR\n");
    }
};

Functor f;
f();
```

A functor is an object, and can be passed around as a parameter
to a function, and held as a standin for a function pointer.  since
it is essentially an object, it can contain state, which might be
useful for the execution of the operator().

PixelTransferOp is a functor interface which supports the creation 
of functions that can be applied per pixel when using various 
graphic interfaces.