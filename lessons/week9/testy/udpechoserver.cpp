/*
https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/

#include <cstdio>

#include "w32.hpp"
#include "Network.hpp"



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

void loop()
{

}

#define PORT 9090
#define MAXLINE 1024

void setup()
{
    char *hostname = "DESKTOP-UUUQ00U";
    char *portname = "9090";



    IPSocket s(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (!s.isValid()) {
        // Could not create socket
        printf("Could not create socket: %s:%s\n", hostname, portname);
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
        memset(inbuff, 0, 512);
        int inLen = s.receiveFrom(&addrFrom, &addrFromLen, inbuff, 512);

        // copy to output buffer
        printf("UDP Server Received [%d]:\n", inLen);
        memcpy(outbuff, inbuff, inLen);

        // send it out
        s.sendTo(&addrFrom, addrFromLen, outbuff, inLen);
    }
}

void main()
{
    preload();
    setup();

    while (true) {
        loop();
    }

    onExit();
}
