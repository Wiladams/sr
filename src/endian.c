#include <stdint.h>
#include <stdio.h>


union foo_t {
    int64_t abiggy;
    int8_t achar;
    int32_t intValue;
    int8_t arrValue[4];
};

// Reference: https://docs.microsoft.com/en-us/cpp/preprocessor/pack?view=vs-2019
// demonstrates what happens with packing
// elements are aligned by the pack size
// For the Person object below, you might 
// expect sizeof(Person) to return 9, but with 
// a packing of 2, the answer should be 10
//
// The way pack works, the compiler will align the
// fields to the smaller of the size of the next field
// or the amount specified in the pack.
//
// In the case below, pack(2) suggests 2 byte alignment
// the double nerd; field is 8 bytes (sizeof(double) == 8)
// two is smaller than 8, so two byte alignment is used.
//
// The age field is aligned to 0, because it's the first field
// The nerd field will start at offset 2, and take up the next 8 bytes
// thus making the whole data structure 10 bytes, instead of 9
#pragma pack(2)
#pragma pack(show)
struct Person {
    int8_t age;
    double nerd;
};

void main()
{
    struct Person p;
    printf("Person size: %zd\n", sizeof(p));

union foo_t foo1 = {0x04030201};

printf("size: %zd\n", sizeof(foo1));
printf("foo1 int: 0x%08x\n", foo1.intValue);
printf("foo1 arr: 0x%02x 0x%02x 0x%02x 0x%02x\n", 
    foo1.arrValue[0],
    foo1.arrValue[1],
    foo1.arrValue[2],
    foo1.arrValue[3]);

}