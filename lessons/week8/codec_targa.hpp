/*
    References
    
    http://www.fileformat.info/format/tga/egff.htm#TGA-DMYID.2
    http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
    
-- library of congress
    https://www.loc.gov/preservation/digital/formats/fdd/fdd000180.shtml

-- other implementations
    https://unix4lyfe.org/targa/
    https://github.com/ftrvxmtrx/tga

    Honors the following properties
    vertical orientation
    horizontal orientation
    truecolor (16, 24, 32)
    indexed (16, 24, 32)
    monochrome (8)
    compressed - RLE, (not RAW)
*/


#include "binstream.hpp"
#include "mmap.hpp"
#include "PixelBufferRGBA32.hpp"

/*
local bitbang = require("bitbang")
local BITSVALUE = bitbang.BITSVALUE
local isset = bitbang.isset
*/


//    Convenience structures
enum class PaletteType : int {
    none   = 0,
    palette     = 1
};

enum class HorizontalOrientation : int {
    LeftToRight = 0,
    RightToLeft = 1
};

enum class VerticalOrientation : int {
    BottomToTop = 0,
    TopToBottom = 1,
};

/*
    Header.ImageDescriptor
//  Bits 7-6 - Data storage interleaving flag.                |
//             00 = non-interleaved.                          |
//             01 = two-way (even/odd) interleaving.          |
//             10 = four way interleaving.                    |
//             11 = reserved.         
*/

enum Interleave : int {
    non_interleaved = 0,
    two_way = 1,
    four_way = 2,
    reserved = 3
};


enum ImageKind : int {
    NoImageData = 0,
    ColorMapped = 1,
    TrueColor = 2,
    Monochrome = 3,
    ColorMappedCompressed = 9,
    TrueColorCompressed = 10,
    MonochromeCompressed = 11,
};


static const int footerSize = 26;
const char * targaXFileID = "TRUEVISION-XFILE";

struct tgaFooter {
    uint32_t    ExtAreaOffset;
    uint32_t    DevDirOffset;
    char        Signature[18];
};

// This is NOT suitable for direct memory
// reading.
struct tgaHeader {
    uint8_t     IDLength;
    uint8_t     ColorMapType;
    uint8_t     ImageType;
    uint16_t    CMapStart;
    uint16_t    CMapLength;
    uint8_t     CMapDepth;
    bool        Compressed;         // calculated

    // Image information
    uint16_t    XOffset;
    uint16_t    YOffset;
    uint16_t    Width;
    uint16_t    Height;
    uint8_t     PixelDepth;         // bits per pixel
    uint8_t     BytesPerPixel;      // calculated
    uint8_t     ImageDescriptor;
};

/*
bool readColorMap(BinStream &bs, tgaHeader &header)
{
    int bytespe = header.CMapDepth / 8;
    local pixtype = ffi.typeof("uint8_t[$]", bytespe)
    local databuff = pixtype()

    local cMap = ffi.new("struct Pixel32[?]", header.CMapLength)

    for (int i= header.CMapStart; i<header.CMapLength) {
        local nRead = bs.readByteBuffer(bytespe, databuff)
        
        if (bytespe == 2) {
            uint16_t src16 = (uint16_t)((databuff[1]<<8) | databuff[0])
            cMap[i].red = lshift(BITSVALUE(src16,0,4),3)
            cMap[i].green = lshift(BITSVALUE(src16,5,9),3)
            cMap[i].blue = lshift(BITSVALUE(src16,10,14),3)
            cMap[i].alpha = 0;
            if BITSVALUE(src16,15,15) >= 1 {
                cMap[i].alpha = 0;  // 255
            }
        } else if (bytespe == 3) {
            cMap[i].red = databuff[0]
            cMap[i].green = databuff[1]
            cMap[i].blue = databuff[2]
            cMap[i].alpha = 0

        } else if (bytespe == 4) {
            cMap[i].red = databuff[0]
            cMap[i].green = databuff[1]
            cMap[i].blue = databuff[2]
            //pix.alpha = databuff[3]   -- We should pre-multiply the alpha?
        }

    }

    return cMap
}
*/

bool isCompressed(tgaHeader &header)
{
    return header.ImageType == ColorMappedCompressed ||
        header.ImageType == TrueColorCompressed ||
        header.ImageType == MonochromeCompressed;
}

bool readHeader(BinStream &bs, tgaHeader &res)
{
    res.IDLength = bs.readOctet();           // 00h  Size of Image ID field
    res.ColorMapType = bs.readOctet();       // 01h  Color map type 
    res.ImageType = bs.readOctet();          // 02h  Image type code
    res.CMapStart = bs.readUInt16();         // 03h  Color map origin
    res.CMapLength = bs.readUInt16();        // 05h  Color map length
    res.CMapDepth = bs.readOctet();          // 07h  Depth of color map entries
    res.Compressed = isCompressed(res);

    // Image Description
    res.XOffset = bs.readUInt16();           // 08h  X origin of image
    res.YOffset = bs.readUInt16();           // 0Ah  Y origin of image
    res.Width = bs.readUInt16();             // 0Ch  Width of image - Maximum 512
    res.Height = bs.readUInt16();            // 0Eh  Height of image - Maximum 482
    res.PixelDepth = bs.readOctet();         // 10h  Number of bits per pixel
    res.BytesPerPixel = res.PixelDepth / 8;
    res.ImageDescriptor = bs.readOctet();    // 11h  Image descriptor byte


/*
                  Image Descriptor Byte.                        |
      Bits 3-0 - number of attribute bits associated with each  |
                   pixel.  For the Targa 16, this would be 0 or |
                   1.  For the Targa 24, it should be 0.  For   |
                   Targa 32, it should be 8.                    |
      Bit 4    - controls left/right transfer of pixels to 
                 the screen.
                 0 = left to right
                 1 = right to left
      Bit 5    - controls top/bottom transfer of pixels to 
                 the screen.
                 0 = bottom to top
                 1 = top to bottom
                 
                 In Combination bits 5/4, they would have these values
                 00 = bottom left
                 01 = bottom right
                 10 = top left
                 11 = top right
                 
      Bits 7-6 - Data storage interleaving flag.                |
                 00 = non-interleaved.                          |
                 01 = two-way (even/odd) interleaving.          |
                 10 = four way interleaving.                    |
                 11 = reserved.         
*/
    //res.AttrBits = (res.ImageDescriptor & 0x0F);
    //res.HorizontalOrientation = ((res.ImageDescriptor & 0x10) >> 4)
    //res.VerticalOrientation = ((res.ImageDescriptor & 0x20) >> 5)
    //res.Interleave = ((res.ImageDescriptor & 0xC0) >> 6)


// If there's an identification section, read that next
//print("ImageIdentification: ", res.IDLength, string.format("0x%x",bs.tell()))
/*
    if (res.IDLength > 0) {
        res.ImageIdentification = bs.readBytes(res.IDLength);
    }


    // If there's a color map, read that next
    if (res.ColorMapType == ColorMapType.Palette) {
        res.ColorMap = readColorMap(bs, res);
    }
*/

    return true;
}




/*
     Targa images come in many different formats, and there are 
     a couple of different versions of the specification.
 
First thing to do is determine if the file is adhereing to version 
2.0 of the spcification.  We do that by reading a 'footer', which 
is the last 26 bytes of the file.

Return a PixelBuffer if we can read the file successfully
*/


bool readFooter(BinStream &bs, tgaFooter)
{
    //print("targa.readFooter, BEGIN")

    rs.ExtensionAreaOffset = bs.readUInt32();
    rs.DeveloperDirectoryOffset = bs.readUInt32();
    rs.Signature = bs.readBytes(16);
    rs.Signature = ffi.string(rs.Signature, 16);
    rs.Period = string.char(bs.readOctet());
    rs.Zero = bs.readOctet();

    rs.isExtended = rs.Signature == targaXFileID

    if (!rs.isExtended) {
        return false;
    }
    //print("targa.readFooter, END")
    
    return rs
}

/*
local TrueColor = ImageType.TrueColor
local Monochrome = ImageType.Monochrome
local ColorMapped = ImageType.ColorMapped
local TrueColorCompressed = ImageType.TrueColorCompressed
local ColorMappedCompressed = ImageType.ColorMappedCompressed
local MonochromeCompressed = ImageType.MonochromeCompressed
*/

local function decodeSinglePixel(pix, databuff, pixelDepth, imtype, colorMap)
    --print(pix, databuff, bpp, imtype)
    if imtype == TrueColor or imtype == TrueColorCompressed then
        if pixelDepth == 24 then
            pix.Red = databuff[0]
            pix.Green = databuff[1]
            pix.Blue = databuff[2]
            pix.Alpha = 0
            return true
        elseif pixelDepth == 32 then
            pix.Red = databuff[0]
            pix.Green = databuff[1]
            pix.Blue = databuff[2]
            --pix.Alpha = databuff[3]   -- We should pre-multiply the alpha?
            return true
        elseif pixelDepth == 16 then
            local src16 = bor(lshift(databuff[1],8), databuff[0])
            pix.Red = lshift(BITSVALUE(src16,0,4),3)
            pix.Green = lshift(BITSVALUE(src16,5,9),3)
            pix.Blue = lshift(BITSVALUE(src16,10,14),3)
            pix.Alpha = 0
            if BITSVALUE(src16,15,15) >= 1 then
                pix.Alpha = 0;  -- 255
            end 
        end
        return true;
    elseif imtype == Monochrome or imtype == MonochromeCompressed then
        pix.Red = databuff[0]
        pix.Green = databuff[0]
        pix.Blue = databuff[0]
        pix.Alpha = 0
        return true
    elseif imtype == ColorMapped or imtype == ColorMappedCompressed then
        -- lookup the color using databuff[0] as index
        local cpix = colorMap[databuff[0]]
        pix.Red = cpix.Red;
        pix.Green = cpix.Green;
        pix.Blue = cpix.Blue;
        return true;
    end

    return false
end

/*
-- We want to figure out the mapping between positions as we
-- read them and their locations in our pixel buffer in one place
-- This iterator figures that out, return x,y pairs for the positions
-- according to the horizontal and vertical orientation.  Ideally this
-- would do interleaving as well, but we don't have an image to test
-- with
*/
local function locations(header)
    local function iterator()
        --  Start with left to right orientation
        local dx = 1;
        local xStart = 0
        local xEnd = header.Width-1
        
        -- switch to right to left of horizontal orientation indicates
        if header.HorizontalOrientation == HorizontalOrientation.RightToLeft then
            dx = -1
            xStart = header.Width-1
            xEnd = 0
        end
    
        -- start top to bottom vertical orientation
        local dy = 1
        local yStart = 0
        local yEnd = header.Height-1
        if header.VerticalOrientation == VerticalOrientation.BottomToTop then
            dy = -1
            yStart = header.Height-1
            yEnd = 0
        end
    
        -- Now do the actual iteration job
        for y = yStart, yEnd, dy do 
            for x=xStart,xEnd, dx do
                coroutine.yield(x,y,pix)
            end
        end
    end

    return coroutine.wrap(iterator)
end

// An iterator which will return the uncompressed
// pixels in row order
local function uncompressedPixels(bs, header)
    local function iterator()
        local bytesPerPixel = header.BytesPerPixel
        local pixtype = ffi.typeof("uint8_t[$]", bytesPerPixel)
        local pix = ffi.new("struct Pixel32")
        local databuff = pixtype()

        for x,y in locations(header) do
            local nRead = bs.readByteBuffer(bytesPerPixel, databuff)
            decodeSinglePixel(pix, databuff, header.PixelDepth, header.ImageType, header.ColorMap)
            coroutine.yield(x,y,pix)
        end
    end

    return coroutine.wrap(iterator)
end

local function compressedPixels(bs, header)
    local function iterator()
        local bytesPerPixel = header.BytesPerPixel
        local pixtype = ffi.typeof("uint8_t[$]", bytesPerPixel)
        local pix = ffi.new("struct Pixel32")
        local databuff = pixtype()
        
        local pixelCount = 0
        local repCount = 0

        for x,y in locations(header) do
            if pixelCount == 0 then
                -- read repeatCount byte to see if it's RLE or RAW
                -- and the number of repetitions                
                repCount = bs.readOctet()
                local isRLE = isset(repCount, 7)
                --print("RLE: ", isRLE)
                pixelCount = BITSVALUE(repCount,0,6) + 1
                local nRead = bs.readByteBuffer(bytesPerPixel, databuff)
                decodeSinglePixel(pix, databuff, header.PixelDepth, header.ImageType, header.ColorMap)
                --print("pixelCount: ", pixelCount, pix)
            end

            coroutine.yield(x,y,pix)
            pixelCount = pixelCount - 1
        end
    end

    return coroutine.wrap(iterator)
end

bool readBody(bs, header)
    --print("targa.readBody, BEGIN")

    local pb = PixelBuffer(header.Width, header.Height)
 
    if not header.Compressed then
        for x,y,pixel in uncompressedPixels(bs, header) do
            pb:set(x,y,pixel)
        end
    else
        for x,y,pixel in compressedPixels(bs, header) do
            pb:set(x,y,pixel)
        end
    end

    return pb
end

-- read a targa image from a stream
-- This assumes other forms, like reading from a fill
-- will create a stream to read from
local function readFromStream(bs, res)
    res = res or {}

    -- position 26 bytes from the end and try 
    -- to read the footer
    bs.seek(bs.size-footerSize)
    local footer, err = readFooter(bs)
    res.Footer = footer

    -- if footer == false, then it's not an extended
    -- format file.  Otherwise, the footer is returned
    -- In either case, time to read the header
    bs.seek(0)
    local header, err = readHeader(bs)
    res.Header = header

    if not header then
        res.Error = "error reading targa headder: "..tostring(err)
        return false, res
    end

    -- We have the header, so we should be able
    -- to read the body
    pixbuff, err = readBody(bs, header)

    res.PixelBuffer = pixbuff
    res.Error = err

    return pixbuff, header, footer
end

PixelBuffer readFromFile(filename)
{
    local filemap, err = mmap(filename)
    if not filemap then
        return false, "file not mapped ()"..tostring(err)
    end

    local bs, err = binstream(filemap:getPointer(), filemap:length(), 0, true )

    if not bs then
        return false, err
    end

    return readFromStream(bs)
}


