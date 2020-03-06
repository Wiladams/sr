#pragma once

#include <stdint.h>

#include "PixelBuffer.hpp"
#include "binstream.hpp"
#include "w32_types.hpp"

// References
// http://entropymine.com/jason/bmpsuite/bmpsuite/
// Specifications
// many former implementations
//
/*
CIEXYZ read_CIEXYZ(BinStream &bs)
{
    CIEXYZ res;

    res.ciexyzX = bs.readF2Dot30();
    res.ciexyzY = bs.readF2Dot30();
    res.ciexyzZ = bs.readF2Dot30();

    return res;
}

CIEXYZTRIPLE read_CIEXYZTRIPLE(BinStream &bs)
{
    CIEXYZTRIPLE res;

    res.ciexyzRed = read_CIEXYZ(bs);
    res.ciexyzGreen = read_CIEXYZ(bs);
    res.ciexyzBlue = read_CIEXYZ(bs);

    return res;
}

/*
local headerFuncs = {
    // BITMAPCOREHEADER, 
    // OS21XBITMAPHEADER
    [12] = function(bs, res)
        res.Width = bs:readWORD();
        res.Height = bs:readWORD();
        res.ColorPlanes = bs:readWORD();
        res.BitsPerPixel = bs:readWORD();
    end;

    // BITMAPCOREHEADER2
    [64] = function(bs, res)
        -- OS22XBITMAPHEADER
    end;

    [16] = function(bs, res)
        -- OS22XBITMAPHEADER
        -- previous format with only first 
        -- 16 bytes valid
    end;

    //BITMAPINFOHEADER 
    [40] = function(bs, res)
        res.Width = bs:readLONG();
        res.Height = bs:readIntLONG();
        res.ColorPlanes = bs:readWORD();
        res.BitsPerPixel = bs:readWORD();
        res.Compression = bs:readDWORD();
        res.ImageSize = bs:readDWORD();
        res.HorizRes = bs:readDWORD();
        res.VertRes = bs:readDWORD();
        res.PaletteEntries = bs:readDWORD();
        res.ImportantColors = bs:readDWORD();
    end;

    // BITMAPV2INFOHEADER 
    [52] = function(bs, res)
    end;

    // BITMAPV3INFOHEADER 
    [56] = function(bs, res)
    end;

    // BITMAPV4HEADER 
    [108] = function (bs, res)
        -- BITMAPINFOHEADER
        res.Width = bs:readIntLONG();
        res.Height = bs:readIntLONG();
        res.ColorPlanes = bs:readWORD();
        res.BitsPerPixel = bs:readWORD();
        res.Compression = bs:readDWORD();
        res.ImageSize = bs:readDWORD();
        res.HorizRes = bs:readDWORD();
        res.VertRes = bs:readDWORD();
        res.PaletteEntries = bs:readDWORD();
        res.ImportantColors = bs:readDWORD();


        res.RedMask = bs:readDWORD();
        res.GreenMask = bs:readDWORD();
        res.BlueMask = bs:readDWORD();
        res.AlphaMask = bs:readDWORD();
        res.CSType = bs:readDWORD();
        res.Endpoints = read_CIEXYZTRIPLE(bs);
        res.GammaRed = bs:readDWORD();
        res.GammaGreen = bs:readDWORD();
        res.GammaBlue = bs:readDWORD();
        
    end;

    // BITMAPV5HEADER 
    [124] = function(bs, res)
        -- BITMAPINFOHEADER
        res.Width = bs:readIntLONG();
        res.Height = bs:readIntLONG();
        res.ColorPlanes = bs:readWORD();
        res.BitsPerPixel = bs:readWORD();
        res.Compression = bs:readDWORD();
        res.ImageSize = bs:readDWORD();
        res.HorizRes = bs:readDWORD();
        res.VertRes = bs:readDWORD();
        res.PaletteEntries = bs:readDWORD();
        res.ImportantColors = bs:readDWORD();

        -- BITMAPV4HEADER
        res.RedMask = bs:readDWORD();
        res.GreenMask = bs:readDWORD();
        res.BlueMask = bs:readDWORD();
        res.AlphaMask = bs:readDWORD();
        res.CSType = bs:readDWORD();
        res.Endpoints = read_CIEXYZTRIPLE(bs);
        res.GammaRed = bs:readDWORD();
        res.GammaGreen = bs:readDWORD();
        res.GammaBlue = bs:readDWORD();


        res.Intent = bs:readDWORD();
        res.ProfileData = bs:readDWORD();
        res.ProfileSize = bs:readDWORD();
        res.Reserved = bs:readDWORD();
    end;
}


local validSignatures = {
    "BM",
    "BA",
    "CI",
    "CP",
    "IC",
    "PT"
};
*/

bool readFileHeader(BinStream &bs, BITMAPFILEHEADER &res)
{
    // Windows Signatur 'BM', 
    // for OS/2 BA, CI, CP, IC, PT
    // assuming it is in fact a file header
    res.bfType = bs.readUInt16();
    // typical value is 0x4D42

    if (res.bfType != 'MB') {
        return false;
    }

    res.bfSize = bs.readUInt32();
    res.bfReserved1 = bs.readUInt16();
    res.bfReserved2 = bs.readUInt16();
    res.bfOffBits = bs.readUInt32();

    return true;
}

/*
local function readImageHeader(bs, res)
    res = res or {}

    -- core header
    res.HeaderSize = bs:readDWORD();

    return res;
end
*/