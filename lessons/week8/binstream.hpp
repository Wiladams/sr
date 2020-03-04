#pragma once

/*
    This file is a general memory stream interface.
    The primary objective is to satisfy the needs of the 
    truetype parser, but it can be used in general cases.

    It differs from the MemoryStream object in that it can't
    write, and it has more specific data type reading 
    convenience calls.

    More specifically, all of the numeric reading assumes the
    data in the stream is formatted in 'big-endian' order.  That
    is, the high order bytes come first.
*/

#include <stdint.h>

//local min = math.min

/*
    Standard 'object' construct.
    __call is implemented so we get a 'constructor'
    sort of feel:
    binstream(data, size, position)
*/
class BinStream
{
    bool fbigend;
    uint8_t *fdata;
    size_t fposition;
    size_t fcursor;
    size_t fsize;

public:
    BinStream(uint8_t *data, size_t size, size_t position, bool littleendian = true)
        : fbigend(!littleendian),
        fposition(0),
        fdata(data),
        fsize(size)
    {
        fcursor = fposition;
    }
 




    // get a subrange of the memory stream
    // returning a new memory stream
    BinStream *range(int64_t size, int64_t pos)
    {
        if (pos < 0 || (fsize < 0)) {
            // BUGBUG - throw exception
            return nullptr;
        }

        if (pos > fsize) {
            // BUGBUG - throw exception
            return nullptr;
        }

        if ((size > (fsize - pos))) { 
            // BUGBUG - throw exception
            return nullptr;
        }

        return new BinStream(fdata+pos, size, 0 , !fbigend);
    }

    BinStream * range(size_t size)
    {
        return range(size, fcursor);
    }

    // report how many bytes remain to be read
    // from stream
    int64_t remaining()
    {
        return fsize - fcursor;
    }

    bool EOF()
    {
        return (remaining() < 1);
    }


 // move to a particular position, in bytes
    bool seek(int64_t pos)
    {
        // if position specified outside of range
        // just set it past end of stream
        if ((pos > fsize)  || (pos < 0)) {
            fcursor = fsize;
            return false;   // , self.cursor;
        } else {
            fcursor = pos;
        }
 
        return true;
    }


    // Report the current cursor position.
    int64_t tell()
    {
        return fcursor;
    }

    // move the cursor ahead by the amount
    // specified in the offset
    // seek, relative to current position
    bool skip(int64_t offset)
    {
        //print("SKIP: ", offset)
        return seek(fcursor + offset);
    }

    // Seek forwad to a boundary of the specified
    // number of bytes.
    bool alignTo(size_t num)
    {
        return skip(fcursor % num);
    }
    
    bool skipToEven()
    {
        return alignTo(2);
    }

    uint8_t * getPositionPointer()
    {
        return fdata + fcursor;
    }

    // get 8 bits, and don't advance the cursor
    uint8_t peekOctet()
    {
        if (fcursor >= fsize) {
            // throw EOF exception
            return false;
        }

        return fdata[fcursor];
    }


    // get 8 bits, and advance the cursor
    uint8_t readOctet()
    {
        //print("self.cursor: ", self.cursor, self.size)
        if (fcursor >= fsize) {
            // BUGBUG - throw exception
            return 0;
        }

        fcursor = fcursor + 1;
    
        return fdata[fcursor-1];
    }


    // Read an integer value
    // The parameter 'n' determines how many bytes to read.
    // 'n' can be up to 8 
    // The routine will deal with big or little endian





/*
-- BUGBUG, do error checking against end of stream
function readBytes(self, n, bytes)
{
    if n < 1 then 
        return false, "must specify more then 0 bytes" 
    end

    -- see how many bytes are remaining to be read
    local nActual = min(n, self:remaining())
    -- read the minimum between remaining and 'n'
    bytes = bytes or ffi.new("uint8_t[?]", nActual)
    local ptr = self.data+self.cursor
    ffi.copy(bytes, ptr, nActual)
    self:skip(nActual)

    -- if minimum is less than n, return false, and the number
    -- actually read
    if nActual < n then
        return false, nActual;
    end

    return bytes, nActual;
}

function readByteBuffer(self, n, buff)
    --print("readByteBuffer, BEGIN: ", n, buff)
    if not buff or n < 1 then 
        return false, "must specify more then 0 bytes" 
    end
    --print("readByteBuffer, 1.0: ")

    -- see how many bytes are remaining to be read
    local nActual = min(n, self:remaining())
    --print("readByteBuffer, 2.0: ")
    -- read the minimum between remaining and 'n'
    local ptr = self.data+self.cursor
    --print("readByteBuffer, 3.0: ", ptr, nActual)
    ffi.copy(buff, ptr, nActual)
    --print("readByteBuffer, 4.0: ")
    self:skip(nActual)
    --print("readByteBuffer, 4.0: ")

    -- if nActual is less than n, return nActual
    if nActual < 1 then
        return false, "EOF"
    end

    --print("readByteBuffer, END")

    return nActual
end


-- Read bytes and turn into a Lua string
-- Read up to 'n' bytes, or up to a '\0' if
-- 'n' is not specified.
function readString(self, n)
    local str = nil;

    --print("BS: ", self:remaining())
    if self:EOF() then
        return false, "EOF"
    end

    if not n then
        -- read to null terminator

        str = ffi.string(self.data+self.cursor)
        --print("binstream, STR: ", str)
        self.cursor = self.cursor + #str + 1;
    else
        -- read a specific number of bytes, turn into Lua string
        str = ffi.string(self.data+self.cursor, n)
        self.cursor = self.cursor + n;
    end

    return str;
end
*/

    uint64_t readNumber(int n)
    {
        uint64_t v = 0;
        int i = 0;

        if (remaining() < n) {
            // BUGBUG - throw exception
            return 0;
        }

        if (fbigend) {
            while  (i < n) {
                v = ((v<< 8) | readOctet());
                i = i + 1;
            }
        } else {
            while  (i < n) {
                v = (v | (readOctet() << 8*i));
                i = i + 1;
            }
        }

        return v;
    }

    // Read 8-bit signed integer
    int8_t readInt8()
    {
        return (int8_t)readNumber(1);
    }

    // Read 8-bit unsigned integer
    uint8_t readUInt8()
    {
        return (uint8_t)readNumber(1);
    }

    // Read 16-bit signed integer
    int16_t readInt16()
    {
        return (int16_t)readNumber(2);
    }

    // Read 16-bit unsigned integer
    uint16_t readUInt16()
    {
        return (uint16_t)readNumber(2);
    }

    // Read Signed 32-bit integer
    int32_t readInt32()
    {
        return (int32_t)readNumber(4);
    }

    // Read unsigned 32-bit integer
    uint32_t readUInt32()
    {
        return (uint32_t)readNumber(4);
    }

    // Read signed 64-bit integer
    int64_t readInt64()
    {
        return (int64_t)readNumber(8);
    }

    uint64_t readUInt64()
    {
        return (uint64_t)readNumber(8);
    }



/*
    Writing to a binary stream
*/
/*
bool writeOctet(octet)
    if (self.cursor >= self.size) then
        return false, "EOF";
    end

    self.data[self.cursor] = octet;
    self.cursor = self.cursor + 1;

    return true;
end

function writeInt(self, value, n)


    if self:remaining() < n then
        return false, "NOT ENOUGH DATA AVAILABLE"
    end

    if self.bigend then
        local i = n-1;
        while  (i >= 0) do
            self:writeOctet(band(rshift(value,i*8), 0xff))
            i = i - 1;
        end 
    else
        local i = 0;
        while  (i < n) do
            self:writeOctet(band(rshift(value,i*8), 0xff))
            i = i + 1;
        end 
    end

    return v;
end

function writeBytes(self,  n, bytes)
    n = n or #bytes
    if n > self:remaining() then 
        return false, "Not enough space"
    end

    ffi.copy(self.data+self.cursor, ffi.cast("const char *", bytes, n))
    self:skip(n)

    return true;
end

function writeString(self, str)
    return self:writeBytes(str)
end

function writeInt8(self, n)
    return self:writeInt(n, 1);
end

function writeUInt8(self, n)
    return self:writeInt(ffi.cast("uint8_t",n), 1)
end

function writeInt16(self, n)
    return self:writeInt(ffi.cast("int16_t",n), 2);
end

function writeUint16(self, n)
    return self:writeInt(ffi.cast("uint16_t",n), 2);
end

function writeInt32(self, n)
    return self:writeInt(ffi.cast("int32_t",n), 4);
end

function writeUInt32(self, n)
    return self:writeInt(ffi.cast("uint32_t",n), 4);
end

function writeInt64(self, n)
    return self:writeInt(ffi.cast("int64_t",n), 8);
end

function writeUInt64(self, n)
    return self:writeInt(ffi.cast("uint64_t",n), 8);
end
*/
/*
// Some various fixed formats
function readFixed(self)
    local decimal = self:readInt16();
    local fraction = self:readUInt16();

    return decimal + fraction / 65535;
end

function readF2Dot14(self)
    return self:readUInt16() / 16384;
end

function readF2Dot30(self)
    return self:readUInt32() / 0x4000000;
end

-- Convenient types named in the documentation
readFWord = readInt16;
readUFWord = readUInt16;
readOffset16 = readUInt16;
readOffset32 = readUInt32;
readWORD = readUInt16;
readDWORD = readUInt32;
readLONG = readInt32;
readBYTE = readOctet;
*/
};