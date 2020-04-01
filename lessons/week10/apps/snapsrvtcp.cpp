/*
https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/


#include <cstdio>

#include "apphost.hpp"
#include "TcpServer.hpp"
#include "binstream.hpp"
#include "screensnapshot.hpp"


static const int MAXBUFF = 1024*2048;
static const char *portname = "9090";
#define APPORT 9090

char inbuff[512];

ScreenSnapshot *snapper = nullptr;


bool sendChunk(IPSocket &s, BufferChunk &bc)
{
    // Get a stream on the chunk
    BinStream chunkStream(bc.fData, bc.fSize);


    int packetCount = 0;

    while (!chunkStream.isEOF()) {
        packetCount = packetCount + 1;

        // we'll write 32K bytes at a time
        // start by writing the number of bytes
        // into the packet header
        int payloadSize = MIN(1400, chunkStream.remaining());


        // Write payload size into packet header
        int sentCode = s.send((const char *)&payloadSize, 4);

        // Write the payload out and advance
        sentCode = s.send((char *)chunkStream.getPositionPointer(), payloadSize);
        chunkStream.skip(payloadSize);
    }

    // Send one more packet of size 0
    char packet[1600];
    BinStream packetStream(packet, 1600);

    packetStream.seek(0);
    packetStream.writeUInt32(0);
    int sentCode = s.send(packet, 4);

    return true;
}

void setup()
{
    snapper = new ScreenSnapshot(0, 0, 800, 600);

    // Create the socket we'll be serving from
    TcpServer srvr(APPORT);

    if (!srvr.isValid()) {
        // Could not create socket
        printf("Could not create server: %d\n", srvr.getLastError());
        halt();
        return;
    }


    // accept a single client
    IPSocket clientSock = srvr.accept(); // wait for a connection

    if (!clientSock.isValid()) {
        printf("clientSock, not valid: %d\n", srvr.getLastError());
        halt();
        return ;
    }

    // while connected
    uint64_t commandCount = 0;
    while (true) {
        // Client sends us a command, so read that
        memset(inbuff, 0, 512);
        int inLen = clientSock.receive(inbuff, 512);
        commandCount = commandCount + 1;
        if (inLen < 0) {
            printf("TCPReceived ERROR: %d\n", WSAGetLastError());
            halt();
            break;
        } else {
            inbuff[inLen] = 0;
            //printf("COMMAND[%Id]: %s\n", commandCount, inbuff);
        }

        // take a snapshot
        snapper->moveNext();
        PixelBufferRGBA32 current = snapper->getCurrent();
        int outLength = current.getDataLength();

        // Send data chunked
        // create binstream on outbuff
        BufferChunk bc(current.getData(), current.getDataLength());
        sendChunk(clientSock, bc);
    }
}
