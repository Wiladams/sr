#pragma once

/*
    Doing realtime math is a useful thing.  Doing it cross platform
    can be very challenging.  The realtime Math project:
    https://github.com/nfrechette/rtm

    does a very good job of creating a specific library to achieve this.
    Herein lies some of the routines borrowed from that particular library.

    Of particular note is the construction of the data structures.  Doing
    alignment to 16-byte boundaries helps achieve faster access to the 
    data structures on most processors.

    Origin:
    https://github.com/nfrechette/rtm/blob/develop/includes/rtm/types.h

*/

#include <cstdint>



namespace fastmath
{
struct alignas(16) quatf
{
    float x;
    float y;
    float z;
    float w;
};

struct alignas(16) vector4f
{
    float x;
    float y;
    float z;
    float w;
};

struct alignas(16) mask4i
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t w;
};

struct alignas(16) mask4q
{
    uint64_t x;
    uint64_t y;
    uint64_t z;
    uint64_t w;
};

using scalard = double;
using scalarf = float;

struct qvff
{
    quatf       rotation;
    vector4f    translation;
    vector4f    scale;
};

struct matrix3x3f
{
    vector4f    x_axis;
    vector4f    y_axis;
    vector4f    z_axis;
};

struct matrix3x4f
{
    vector4f    x_axis;
    vector4f    y_axis;
    vector4f    z_axis;
    vector4f    w_axis;
};

struct matrix3x4f
{
    vector4f    x_axis;
    vector4f    y_axis;
    vector4f    z_axis;
    vector4f    w_axis;
};

enum class mix4
{
    x = 0,
    y = 1,
    z = 2,
    w = 3,

    a = 4,
    b = 5,
    c = 6,
    d = 7
};

enum class axis4
{
    x = 0,
    y = 1,
    z = 2,
    w = 3
};

class anglef;
class angled;

// Various unaligned types suitable for interop
struct float2f
{
    float x;
    float y;
};

struct float3f
{
    float x;
    float y;
    float z;
};

struct float4f
{
    float x;
    float y;
    float z;
    float w;
};

}