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
#define PORT 9090

char inbuff[512];

ScreenSnapshot *snapper = nullptr;


bool sendChunk(IPSocket &s, BufferChunk &bc)
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
        int sentCode = s.send((const char *)&payloadSize, 4);

        // Write the payload out and advance
        sentCode = s.send((char *)chunkStream.getPositionPointer(), payloadSize);
        chunkStream.skip(payloadSize);
    }

    // Send one more packet of size 0
    packetStream.seek(0);
    packetStream.writeUInt32(0);
    int sentCode = s.send(packet, 4);

    return true;
}

void setup()
{
    snapper = new ScreenSnapshot(0, 0, 800, 600);

    // Create a server address
    //struct sockaddr_in servaddr;
    //memset(&servaddr, 0, sizeof(servaddr));
    //servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.S_addr = INADDR_ANY;  // we don't care about address
    //servaddr.sin_port = htons(PORT);

    // Create the socket we'll be serving from
    TcpServer srvr("localhost", PORT);

    if (!srvr.isValid()) {
        // Could not create socket
        printf("Could not create server: %d\n", srvr.getLastError());
        return;
    }

    if (!srvr.bind()) {
        printf("failed to bind: %d\n", srvr.getLastError());
        halt();
        return ;
    };

    srvr.makePassive();    // listen


    IPSocket clientSock = srvr.accept(); // wait for a connection

    if (!clientSock.isValid()) {
        printf("clientSock, not valid: %d\n", srvr.getLastError());
        halt();
        return ;
    }

    // while connected
    while (true) {
        // Client sends us a command, so read that
        memset(inbuff, 0, 512);
        int inLen = clientSock.receive(inbuff, 512);
        if (inLen < 0) {
            printf("TCPReceived ERROR: %d\n", WSAGetLastError());
            halt();
            break;
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
        sendChunk(clientSock, bc);
    }
}
