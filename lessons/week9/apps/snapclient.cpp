#include "p5.hpp"

#include <cstdio>
#include <cstring>

#include "Network.hpp"
#include "PixelBufferRGBA32.hpp"
#include "binstream.hpp"

#define PORT 9090
static const int  MAXBUFF = 1024 * 2048;

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



IPSocket * s = nullptr;
IPHost * host = nullptr;
IPAddress *servaddr = nullptr;
char *msg = "GET / HTTP/1.1\r\n";
char inBuff[MAXBUFF];

bool receiveChunk(IPSocket *s, struct sockaddr * addrFrom, int *addrLen, BinStream &pixs)
{
    char packet[1600];
    BinStream packetStream(packet, 1600);
    int packetCount = 0;

    while (!pixs.isEOF()) {
        packetCount = packetCount + 1;

        //packetStream.seek(0);
        // receive a single packet
        int payloadSize;
        int recvLen = s->receiveFrom(addrFrom, addrLen, (char *)&payloadSize, 4);
        //printf("Message Received: (%d) [%d]  Payload Size: %d\n", packetCount, recvLen, payloadSize);

        if (recvLen < 0) {
            return false;
        }

        // Read the payload size out of the packet
        // payload size is a 32-bit unsigned integer at the
        // beginning of the packet
        //int payloadSize = packetStream.readUInt32();
        //printf("Payload size: (%d) %d\n", packetCount, payloadSize);

        // We've reached the end of the whole message
        // with a zero sized payload
        if (payloadSize == 0) {
            //printf("== EOM ==");
            break;
        }

        // Now read the rest of the payload into the 
        // pixs stream
        recvLen = s->receiveFrom(addrFrom, addrLen, (char *)pixs.getPositionPointer(), payloadSize);
        pixs.skip(recvLen);
        //pixs.writeBytes(payloadSize, (const uint8_t *)packetStream.getPositionPointer());
        //printf("  Payload: (%d) [%d]  Payload Size: %d\n", packetCount, recvLen, payloadSize);

    }

    return true;
}

void draw()
{
    // Send a command to the server to grab
    // a snapshot of the screen
    int msgLen = strlen(msg);
    int sentCode = s->sendTo(servaddr->fAddress, servaddr->fAddressLength, msg, msgLen);
    //printf("Message Sent: %d ==> %d\n", sentCode, WSAGetLastError());

    // Receive a chunk back from the server
    // and assemble it into a PixelBuffer
    BinStream pixs(inBuff, MAXBUFF);
    int servLen = servaddr->fAddressLength;
    if (receiveChunk(s, servaddr->fAddress, &servLen, pixs)) {
        // Wrap the data in a PixelBuffer interface
        PixelBufferRGBA32 pb(640, 480, inBuff);

        // display it
        image(pb, 0,0);
    }
}

void setup()
{
    preload();

    createCanvas(800,600);

    char *hostname = "DESKTOP-UUUQ00U";
    char *portname = "9090";

    // Create socket object
    s = new IPSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Get server address
    host = IPHost::create(hostname, portname, AF_INET, SOCK_DGRAM);
    servaddr = host->getAddress();
}


