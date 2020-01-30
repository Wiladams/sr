local ffi = require("ffi")

ffi.cdef[[
#pragma pack(4)

struct Person {
    int8_t age;
    double nerd;
};
]]

print("Person Size: ", ffi.sizeof("struct Person"))
