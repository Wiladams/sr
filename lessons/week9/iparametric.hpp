#pragma once

#include <stdio.h>

/*
    Templatized version of linear interpolation function.
    This allows any kind of values to be used, not just numeric.

    The requirements of the class to be used as the type is 
    that it supports the following operators: add, sub, mul, div
*/

template <typename T>
inline T IMAP(T x, T olow, T ohigh, T rlow, T rhigh)
{
    return rlow + (x-olow)*((T)(rhigh-rlow)/(ohigh-olow));
}

/*
    IParametric is a templatized functor interface for parametric
    functions.

    A parametric function is one that takes a parameter, or a couple and
    returns a single value.  It is typical that the function will return
    the same output given the same input.
*/
template <typename T>
class IParametric
{
public:
    virtual T operator()(const T &u) = 0;
};

// linear interpolation of values
// as a functor.
template <typename T>
class ValueInterpolator : public IParametric<T>
{
public:
    T fInLow;
    T fInHigh;
    T fOutLow;
    T fOutHigh;

public:
    ValueInterpolator(T inLow, T inHigh, T outlow, T outhigh)
        : fInLow(inLow),
        fInHigh(inHigh),
        fOutLow(outlow),
        fOutHigh(outhigh)
    {}

    T operator()(const double &u)
    {
        //printf("VI: %f %f %f %f %f\n", u, fInLow, fInHigh, fOutLow, fOutHigh);
        T value = IMAP(u, fInLow,fInHigh, fOutLow, fOutHigh);
        return value;
    }
};