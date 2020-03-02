#pragma once

/*
    A functor is simply an object that's standing in as a function.
    It differs from a plain function pionter because it can contain
    persistant state information.

    In C++ terms, functors can be easily implemented by overloading
    the '()' operator.  Then an instance can be 'called' by using
    what looks like standard function call semantics.

    example:
    struct ToGray {
        PixRGBA operator()(const PixRGBA &c){
            PixRGBA c = {c.red, c.red, c.red,255};
            return c;
        }
    }

    ToGray grayifier;

    foreach(pixel in pixels) {
        setPixel(grayifier(pixel));
    }
*/

// Just an example functor that does nothing.
// The key is to implement an operator() that has the
// desired function call signature.
struct Functor
{
    void operator()(void)
    {
        // do something interesting here
        // printf("FUNCTOR\n");
    }
};
