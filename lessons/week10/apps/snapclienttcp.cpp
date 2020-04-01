#include "p5.hpp"

#include <cstdio>
#include <cstring>


#include "PixelBufferRGBA32.hpp"
#include "binstream.hpp"
#include "TcpClient.hpp"



TcpClient * s = nullptr;

static const int  MAXBUFF = 1024 * 2048; // (800x600 maximum)
char *msg = "GET / HTTP/1.1\r\n";
char inBuff[MAXBUFF];
char hostname[256];
char *portname = "9090";


// Receive a chunk of stuff back from the server
// Do this in a loop until the chunk size is 0
// or we've exhausted space in the binstream
bool receiveChunk(TcpClient *s, BinStream &pixs)
{
    int packetCount = 0;

    // make sure we don't run off the end
    // of the chunk buffer
    while (!pixs.isEOF()) {
        packetCount = packetCount + 1;

        // First step is to receive back a 32-bit uint32
        // this indicates the size of the payload
        int payloadSize;
        int recvLen = s->receive((char *)&payloadSize, 4);
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
        recvLen = s->receive((char *)pixs.getPositionPointer(), payloadSize);
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
    int sentCode = s->send(msg, msgLen);
    //printf("Message Sent: %d ==> %d\n", sentCode, WSAGetLastError());

    // Receive a chunk back from the server
    // and assemble it into a PixelBuffer
    BinStream pixs(inBuff, MAXBUFF);

    if (receiveChunk(s, pixs)) {
        // Wrap the data in a PixelBuffer interface
        PixelBufferRGBA32 pb(800, 600, inBuff);

        // display it
        image(pb, 0,0);
    }
}

void setup()
{
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
    s = new TcpClient(hostname, portname);

    if (!s->isValid()) {
        printf("TcpClient ERROR: %d", s->getLastError());
        halt();
        return ;
    }


}


