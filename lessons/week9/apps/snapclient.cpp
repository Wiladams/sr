#include "p5.hpp"

#include <cstdio>
#include <cstring>

#include "Network.hpp"
#include "PixelBufferRGBA32.hpp"
#include "binstream.hpp"

//#define PORT 9090
static const int  MAXBUFF = 1024 * 2048; // (800x600 maximum)
IPSocket * s = nullptr;
IPHost * host = nullptr;
IPAddress *servaddr = nullptr;
char *msg = "GET / HTTP/1.1\r\n";
char inBuff[MAXBUFF];
char hostname[256];
char *portname = "9090";


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

// Receive a chunk of stuff back from the server
// Do this in a loop until the chunk size is 0
bool receiveChunk(IPSocket *s, struct sockaddr * addrFrom, int *addrLen, BinStream &pixs)
{
    int packetCount = 0;

    // make sure we don't run off the end
    // of the chunk buffer
    while (!pixs.isEOF()) {
        packetCount = packetCount + 1;

        // First step is to receive back a 32-bit uint32
        // this indicates the size of the payload
        int payloadSize;
        int recvLen = s->receiveFrom(addrFrom, addrLen, (char *)&payloadSize, 4);
        //printf("Message Received: (%d) [%d]  Payload Size: %d\n", packetCount, recvLen, payloadSize);

        // if we had an error in receiving, then we return immediately
        // indicating we did not read anything
        if (recvLen < 0) {
            return false;
        }


        if (payloadSize == 0) {
            // We've reached a zero sized payload
            // we're at the end of message
            // so break out of the loop
            //printf("== EOM ==");
            break;
        }

        // Now read the rest of the payload into the 
        // pixs stream
        recvLen = s->receiveFrom(addrFrom, addrLen, (char *)pixs.getPositionPointer(), payloadSize);
        pixs.skip(recvLen);
        //printf("  Payload: (%d) [%d]  Payload Size: %d\n", packetCount, recvLen, payloadSize);
    }

    return true;
}

/*
    For each frame as per frameRate
*/
void draw()
{
    // First send a command to the server to get things going
    // the message should cause the server to respond by sending
    // back a current screen snapshot
    int msgLen = strlen(msg);
    int sentCode = s->sendTo(servaddr->fAddress, servaddr->fAddressLength, msg, msgLen);
    //printf("Message Sent: %d ==> %d\n", sentCode, WSAGetLastError());

    // Receive a chunk back from the server
    // and assemble it into a PixelBuffer
    BinStream pixs(inBuff, MAXBUFF);
    int servLen = servaddr->fAddressLength;
    if (receiveChunk(s, servaddr->fAddress, &servLen, pixs)) {
        // Wrap the data in a PixelBuffer interface
        PixelBufferRGBA32 pb(800, 600, inBuff);

        // display it
        image(pb, 0,0);
    }
}

void setup()
{
    preload();

    createCanvas(800,600);
    frameRate(15);

    if (gargc > 1) {
        strcpy(hostname, gargv[1]);
    } else
    {
        strcpy(hostname, "localhost");
    }
    
    printf("HOST: %s\n", hostname);

    // Create socket object
    s = new IPSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Get server address
    host = IPHost::create(hostname, portname, AF_INET, SOCK_DGRAM);
    servaddr = host->getAddress();
}


