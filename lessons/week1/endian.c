#include <stdint.h>
#include <stdio.h>

union foo_t {
    int32_t intValue;
    int8_t arrValue[4];
};
void main()
{
    union foo_t foo1 = {0x04030201};

printf("size: %zd\n", sizeof(foo1));
printf("foo1 int: 0x%08x\n", foo1.intValue);
printf("foo1 arr: 0x%02x 0x%02x 0x%02x 0x%02x\n", 
    foo1.arrValue[0],
    foo1.arrValue[1],
    foo1.arrValue[2],
    foo1.arrValue[3]);
}