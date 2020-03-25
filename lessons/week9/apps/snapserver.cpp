/*
https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/


#include <cstdio>

#include "Network.hpp"
#include "binstream.hpp"
#include "screensnapshot.hpp"


#define PORT 9090
static const int MAXBUFF = 1024*2048;

char inbuff[512];

ScreenSnapshot *snapper = nullptr;

void preload()
{
    uint16_t version = MAKEWORD(2,2);
    WSADATA lpWSAData;

    int res = WSAStartup(version, &lpWSAData);
    printf("WSAStartup: %d\n", res);
}


// When we're about to exit
// do any cleanup
void onExit()
{
    WSACleanup();
}

bool sendChunk(IPSocket &s, BufferChunk &bc, struct sockaddr *addrTo, const int addrToLen)
{
    char packet[1600];
    // Get a stream on the chunk
    BinStream chunkStream(bc.fData, bc.fSize);

    // Get a stream on the packet
    BinStream packetStream(packet, 1600);

    int packetCount = 0;

    while (!chunkStream.isEOF()) {
        packetCount = packetCount + 1;

        // we'll write 1400 bytes at a time
        // start by writing the number of bytes
        // into the packet header
        int payloadSize = MIN(1400, chunkStream.remaining());


        // Write payload size into packet header
        int sentCode = s.sendTo(addrTo, addrToLen, (const char *)&payloadSize, 4);

        // Write the payload out and advance
        sentCode = s.sendTo(addrTo, addrToLen, (char *)chunkStream.getPositionPointer(), payloadSize);
        chunkStream.skip(payloadSize);
    }

    // Send one more packet of size 0
    packetStream.seek(0);
    packetStream.writeUInt32(0);
    int sentCode = s.sendTo(addrTo, addrToLen, packet, 4);

    return true;
}

void setup()
{
    snapper = new ScreenSnapshot(0, 0, 800, 600);

    // Create the socket we'll be serving from
    IPSocket s(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (!s.isValid()) {
        // Could not create socket
        printf("Could not create socket: %d\n", WSAGetLastError());
        return;
    }

    // Create a server address
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.S_addr = INADDR_ANY;  // we don't care about address
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    int bindCode = s.bindTo((const sockaddr *)&servaddr, (const int)sizeof(servaddr));
    printf("Bind code: %d\n", bindCode);


    struct sockaddr addrFrom;
    int addrFromLen = sizeof(addrFrom);

    while (true)
    {
        // Client sends us a command
        memset(inbuff, 0, 512);
        int inLen = s.receiveFrom(&addrFrom, &addrFromLen, inbuff, 512);
        if (inLen < 0) {
            printf("UDPReceived ERROR: %d\n", WSAGetLastError());
            continue;
        } else {
            inbuff[inLen] = 0;
            //printf("COMMAND: %s\n", inbuff);
        }


        // take a snapshot
        snapper->moveNext();
        PixelBufferRGBA32 current = snapper->getCurrent();
        int outLength = current.getDataLength();

        // Send data chunked
        // create binstream on outbuff
        BufferChunk bc(current.getData(), current.getDataLength());
        sendChunk(s, bc, &addrFrom, addrFromLen);
    }
}

void main()
{
    preload();
    setup();
    onExit();
}
