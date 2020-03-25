/*
    STUN
    RFC 5389 https://tools.ietf.org/html/rfc5389#page-5
    Port: 3478
*/

#include "w32.hpp"
#include "binstream.hpp"



struct STUNHeader {
    uint16_t fMessageType;
    uint16_t fMessageLength;
    uint32_t fCookie;
    uint8_t fTransactionID[12];

    STUNHeader()
    {
        fMessageType = 0;
        fMessageLength = 0;
        fCookie = 0x2112A442;
    }

    // BinStream should be big-endian
    bool write(BinStream &bs)
    {
        bs.writeUInt16(fMessageType);
        bs.writeUInt16(fMessageLength);
        bs.writeUInt32(fCookie);
        bs.writeBytes(sizeof(fTransactionID), fTransactionID);

        return true;
    }
}


void preload()
{
    uint16_t version = MAKEWORD(2,2);
    WSADATA lpWSAData;

    int res = WSAStartup(version, &lpWSAData);
    printf("WSAStartup: %d\n", res);
    printf(" Max Sockets: %d\n", lpWSAData.iMaxSockets);
    printf("  Max Udp Dg: %d\n", lpWSAData.iMaxUdpDg);
    printf("      Vendor: %s\n", lpWSAData.lpVendorInfo);
    printf(" Description: %s\n", lpWSAData.szDescription);
    printf("      Status: %s\n", lpWSAData.szSystemStatus);
    

}

// When we're about to exit
// do any cleanup
void onExit()
{
    WSACleanup();
}

void setup()
{

}

void main()
{
    preload();
    setup();
    onExit();
}