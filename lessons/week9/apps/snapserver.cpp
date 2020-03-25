/*
https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/

#include "p5.hpp"

#include <cstdio>

#include "w32.hpp"
#include "Network.hpp"

#include "screensnapshot.hpp"

ScreenSnapshot *ss = nullptr;

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

#define PORT 9090
#define MAXBUFF 1024*2048;

// When we're about to exit
// do any cleanup
void onExit()
{
    WSACleanup();
}

void setup()
{
    preload();

    ss = new ScreenSnapshot(0, 0, 1024, 1080);

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

    char outbuff[512];
    char inbuff[512];
    struct sockaddr addrFrom;
    int addrFromLen;

    while (true)
    {
        // Client sends us a command
        memset(inbuff, 0, 512);
        int inLen = s.receiveFrom(&addrFrom, &addrFromLen, inbuff, 512);
        printf("UDP Server Received [%d]:\n", inLen);


        // decode the command
        // execute the command
        // take a snapshot
        ss->moveNext();
        PixelBufferRGBA32 current = ss->getCurrent();
        int outLength = current.getDataLength();
        memcpy(outbuff, current.getData(), outLength);

        // send it out
        s.sendTo(&addrFrom, addrFromLen, outbuff, inLen);
    }

    onExit();
}



