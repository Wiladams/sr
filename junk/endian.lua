--[[
    Demonstration of what "endian means"
]]

local ffi = require("ffi")

ffi.cdef[[
union foo {
    int32_t intValue;
    uint8_t arrValue[4];
};
]]
local fooU = ffi.typeof("union foo")

local foo1 = fooU(0x04030201)

print("size: ", ffi.sizeof(foo1))
print(string.format("foo1 int: 0x%08x", foo1.intValue))
print(string.format("foo1 arr: 0x%02x 0x%02x 0x%02x 0x%02x", 
    foo1.arrValue[0],
    foo1.arrValue[1],
    foo1.arrValue[2],
    foo1.arrValue[3]))